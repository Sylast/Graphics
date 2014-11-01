#include "framebuffer.h"
#include "vector.h"
#include <iostream>
#include "scene.h"

FrameBuffer::FrameBuffer(int u0, int v0, int _w, int _h) 
	: Fl_Gl_Window(u0, v0, _w, _h, 0) {

		w = _w;
		h = _h;
		pix = new unsigned int[w*h];
		zb = new float[w*h];
		isHW = false;

}

void FrameBuffer::draw() {

	glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, pix);
	if(isHW) {
		if(!scene->HWinit) {
			glEnable(GL_DEPTH_TEST);
			scene->InitializeTextures();
			scene->InitializeLights();
			scene->HWinit = true;
		}
		scene->RenderHW();
	}
}

void FrameBuffer::Set(unsigned int bgr) {
	for (int i = 0; i < w*h; i++) {
		pix[i] = bgr;
	}

}

void FrameBuffer::Set(int u, int v, unsigned int color) {

	pix[(h-1-v)*w+u] = color;

}

unsigned int FrameBuffer::Get(int u, int v) {

	return pix[(h-1-v)*w+u];

}

void FrameBuffer::SetSafe(int u, int v, unsigned int color) {

	if (u < 0 || u > w-1 || v < 0 || v > h-1)
		return;

	Set(u, v, color);

}

void FrameBuffer::SetChecker(int csize, unsigned int color0, unsigned int color1) {

	for (int v = 0; v < h; v++) {
		int cv = v / csize;
		for (int u = 0; u < w; u++) {
			int cu = u / csize;
			if ( (cu+cv) % 2)
				Set(u, v, color1);
			else
				Set(u, v, color0);
		}
	}

}

void FrameBuffer::DrawPoint(int u, int v, int psize, unsigned int color) {

	for (int i = v - psize/2; i <= v + psize/2; i++) {
		for (int j = u - psize/2; j <= u + psize/2; j++) {
			SetSafe(j, i, color);
		}
	}

}

void FrameBuffer::DrawSegment(vector pp0, vector pp1, unsigned int color) {

	float u0f = pp0[0];
	float u1f = pp1[0];
	float v0f = pp0[1];
	float v1f = pp1[1];

	float du = fabsf(u1f-u0f);
	float dv = fabsf(v1f-v0f);
	int stepsN = (du < dv) ? 1 + (int) dv : 1 + (int) du;

	vector startingPoint(u0f, v0f, pp0[2]);
	vector endingPoint(u1f, v1f, pp1[2]);
	int segsN;
	if (stepsN == 1)
		segsN = 1;
	else
		segsN = stepsN-1;
	vector segmentStep = (endingPoint - startingPoint) / (float) segsN;
	int i;
	vector currentPoint;
	for (	i = 0,
			currentPoint = startingPoint;
			i < stepsN;
			i++,
			currentPoint = currentPoint + segmentStep
		) {
		int u = (int) currentPoint[0];
		int v = (int) currentPoint[1];
		if (IsOutsideFrame(u,v))
			continue;
		if (IsFarther(u, v, currentPoint[2]))
			continue;
		SetZ(u, v, currentPoint[2]);
		Set(u, v, color);
	}
}

void FrameBuffer::DrawCircle(float h, float k, float rx, float ry, unsigned int color, bool fill) {
	vector center(h, k, 0.0f);
	vector edge(rx, ry, 0.0f);

	float a = .01f;
	int steps = 360/a;
	vector oldEdge = edge;
	for (int i = 0; i < steps; i++) {
		edge.rotatePoint(center, vector(h,k,1.0f), a);	
		DrawSegment(oldEdge, edge, color);
		if(fill)DrawSegment(center, edge, color);
		oldEdge = edge;
	}  
}

void FrameBuffer::Draw3DPoint(vector pt, PPC *ppc, int psize, vector color) {

	vector ppt;
	if (!ppc->Project(pt, ppt))
		return;
	DrawPoint((int)ppt[0], (int)ppt[1], psize, color.GetColor());
}


void FrameBuffer::Draw3DSegment(vector p0, vector p1, PPC *ppc, unsigned int color) {

	vector pp0, pp1;
	if (!ppc->Project(p0, pp0))
		return;
	if (!ppc->Project(p1, pp1))
		return;
	DrawSegment(pp0, pp1, color);  

}

void FrameBuffer::SetZ(int u, int v, float currz) {
	zb[ (h-1-v)*w+u ] = currz;
}

bool FrameBuffer::IsFarther(int u, int v, float currz) {
	if (zb[(h-1-v)*w+u] >= currz)
		return true;
	return false;
}

bool FrameBuffer::IsOutsideFrame( int u, int v ) {
	
	bool ret;
	ret = u < 0 || u > w-1 || v < 0 || v > h-1;
	return ret;
	
}

void FrameBuffer::Clear(unsigned int bgr, float z0) {
	Set(bgr);
	for (int i = 0; i < w*h; i++) {
		zb[i] = z0;
	}
}

void FrameBuffer::GLFrameClear() {
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT |
			GL_DEPTH_BUFFER_BIT |
			GL_STENCIL_BUFFER_BIT);
}

void FrameBuffer::Convolve33(matrix kernel, FrameBuffer *&fb1) {

	for (int v = 1; v < h-1; v++) {
		for (int u = 1; u < w-1; u++) {
			vector newColor(0.0f, 0.0f, 0.0f);
			for (int vi = -1; vi <= 1; vi++) {
				for (int ui = -1; ui <= 1; ui++) {
					vector currColor;
					currColor.SetFromColor(fb1->Get(u+ui, v+vi));
					newColor = newColor + currColor * kernel[ui+1][vi+1];
				}
			}
			unsigned int newc = newColor.GetColor();
			this->Set(u, v, newc);
		}
	}

}