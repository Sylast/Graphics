
#include "ppc.h"
#include "matrix.h"
#include "framebuffer.h"

PPC::PPC(float hfov, int _w, int _h) : w(_w), h(_h) {

  C = vector(0.0f, 0.0f, 0.0f);
  a = vector(1.0f, 0.0f, 0.0f);
  b = vector(0.0f, -1.0f, 0.0f);
  float alpha = hfov / 2.0f * 3.1415926f / 180.0f;
  c = vector(-(float)w / 2.0f, (float)h / 2.0f, -(float)w / (2.0f *tanf(alpha)));

}


bool PPC::Project(vector P, vector &PP) {

  matrix cam;
  cam.setCol(0,a);
  cam.setCol(1,b);
  cam.setCol(2,c);

  vector q = invert(cam) * (P-C);

  PP[0] = PP[1] = PP[2] = FLT_MAX;

  if (q[2] <= 0.0f) {
    return false;
  }

  PP[0] = q[0] / q[2];
  PP[1] = q[1] / q[2];
  PP[2] = 1.0f / q[2];
  return true;

}

vector PPC::UnProject(vector pp) {
	vector ret;
	ret = C + (a*pp[0] + b*pp[1] +c)/pp[2];
	return ret;
}

void PPC::PositionAndOrient(vector newC, vector lap, vector vpv) {


  vector newVD = (lap - newC).norm();

  vector newa = (newVD ^ vpv).norm();
  newa = newa * a.length();

  vector newb = (newVD ^ newa).norm();
  newb = newb * b.length();

  vector newc = newVD*GetF() -newa*(float)w/2.0f
    -newb*(float)h/2.0f;

  a = newa;
  b = newb;
  c = newc;
  C = newC;

}

vector PPC::GetVD() {

  vector ret;
  ret = (a ^ b).norm();
  return ret;

}

float PPC::GetF() {

  float ret;
  ret = GetVD()*c;
  return ret;

}

vector PPC::GetRayForPix(int u, int v) {
	vector ret;
	ret = a*(u+0.5f)+b*(v+0.5f)+c;
	return ret;
}

vector PPC::GetRayForPIP(float uf, float vf) {
	vector ret;
	ret = a*uf + b*vf + c;
	return ret;
}

vector PPC::GetPC(int u, int v) {
	vector ret;
	ret = C + GetRayForPix(u, v);
	return ret;
}

float PPC::Gethfov() {
	return 2 * atan(w/2 * a.length() / GetF());	
}

vector PPC::GetPP() {
	vector ret;
	ret[0] = (c*-1) * a.norm() / a.length();
	ret[1] = (c*-1) * b.norm() / b.length();
	ret[2] = 0;
	return ret;
}

vector PPC::GetPointOnFocalPlane(float uf, float vf, float f) {

  float of = GetF();
  vector ret;
  ret = C + (c + a*uf + b*vf)*f / of;
  return ret;

}

//Give Unit Vector * distance
void PPC::translate(int direction, float sf) {
	switch(direction) {
		case PPC_UP:
			C= C - b.norm()*sf;
			return;
		case PPC_DOWN:
			C= C + b.norm()*sf;
			return;
		case PPC_LEFT:
			C= C - a.norm()*sf;
			return;
		case PPC_RIGHT:
			C= C + a.norm()*sf;
			return;
		case PPC_FORWARD:
			C= C + (a^b).norm()*sf;
			return;
		case PPC_BACKWARD:
			C= C - (a^b).norm()*sf;
			return;
	}
}

void PPC::rotate(int type, float sf) {
	vector _a = a;
	vector _b = b;
	vector _c = a^b;
	vector d;
	switch (type) {
		case PPC_PAN:
			d = (C-b) - C;
			a.rotateVector(d, sf);
			b.rotateVector(d, sf);
			c.rotateVector(d, sf);
			return;
		case PPC_TILT:
			d = (C+a) - C;
			a.rotateVector(d, sf);
			b.rotateVector(d, sf);
			c.rotateVector(d, sf);
			return;
		case PPC_ROLL:
			d = (C+_c) - C;
			a.rotateVector(d, sf);
			b.rotateVector(d, sf);
			c.rotateVector(d, sf);
			return;
	}
}

void PPC::zoom(float sf) {
	float df = GetF()*(1+sf);
	c = a * -GetPP()[0] - b * GetPP()[1] + GetVD()*df;
}

void PPC::position(vector lap, float d, vector vd) {
	vector up = b * -1;
	C = lap - vd*d;
	a = (vd ^ up).norm() * a.length();
	b = (vd ^ up).norm() * b.length();
	c = a * -GetPP()[0] + b * -GetPP()[1] + vd * GetF();

}

PPC* PPC::interpolate(PPC* endC, vector subject, float f) {
	PPC *oldC(this);
	PPC* newC(this);
	int N = 100;
	int i = N*f;
	newC->C = oldC->C + (endC->C - oldC->C) * (float)i / (float)(N-1); 
	newC->a = oldC->a + (endC->a - oldC->a) * (float)i / (float)(N-1); 
	newC->b = oldC->b + (endC->b - oldC->b) * (float)i / (float)(N-1); 
	newC->c = oldC->c + (endC->c - oldC->c) * (float)i / (float)(N-1); 

	newC->PositionAndOrient(newC->C, subject, vector(0,1,0)); 

	return newC;
}

void PPC::SetByInterpolation(PPC* ppc0, PPC* ppc1, float frac) {

  vector newC = ppc0->C + (ppc1->C - ppc0->C)*frac;
  vector vd0 = ppc0->a ^ ppc0->b;
  vector vd1 = ppc1->a ^ ppc1->b;
  vector newvd = (vd0 + (vd1 - vd0)*frac).norm();
  vector vpv = (ppc0->b + (ppc1->b - ppc0->b)*frac).norm()*-1.0f;
  PositionAndOrient(newC, newC + newvd, vpv);

}

void PPC::RenderWireframe(PPC *ppc, FrameBuffer *fb, float f, unsigned int color) {
	vector baseCorners[4];
	baseCorners[0] = GetPointOnFocalPlane(0.0f, 0.0f, f); 
	baseCorners[1] = GetPointOnFocalPlane(0.0f, (float)h, f); 
	baseCorners[2] = GetPointOnFocalPlane((float)w, (float)h, f); 
	baseCorners[3] = GetPointOnFocalPlane((float)w, 0.0f, f); 
	for (int si = 0; si < 4; si++) {
		fb->Draw3DSegment(C, baseCorners[si], ppc, color);
		fb->Draw3DSegment(baseCorners[si], baseCorners[(si+1)%4], ppc, color);
	}
}

void PPC::SetIntrinsics(float zNear, float zFar) {
	
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float scf = zNear / GetF();
	float left = -(float)w/2.0f*scf;
	float right = (float)w/2.0f*scf;
	float bottom = -(float)h/2.0f*scf;
	float top = (float)h/2.0f*scf;
	glFrustum(left, right, bottom, top, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);

}

void PPC::SetExtrinsics() {
	vector lap = C + GetVD();
	vector up = -b;
	glLoadIdentity();
	gluLookAt(C[0], C[1], C[2], lap[0], lap[1], lap[2], up[0], up[1], up[2]);
}
