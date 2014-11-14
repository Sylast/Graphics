#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <GL/glut.h>
#include "matrix.h"
#include "ppc.h"

class FrameBuffer : public Fl_Gl_Window {
public:
	unsigned int *pix;
	float *zb;
	int w, h;
	bool isHW;
	FrameBuffer(int u0, int v0, int _w, int _h);

	unsigned int GetColor(int x, int y);

	void draw();
	void drawHW();
	void Set(unsigned int bgr);
	void Set(int u, int v, unsigned int color);
	unsigned int Get(int u, int v);
	void SetSafe(int u, int v, unsigned int color);
	void SetChecker(int csize, unsigned int color0, unsigned int color1);
	void DrawPoint(int u, int v, int psize, unsigned int color);
	void DrawSegment(vector pp0, vector pp1, unsigned int color);
	void DrawRect(float u0f, float v0f, float u1f, float v1f,
		unsigned int color, bool fill);
	void DrawCircle(float h, float k, float rx, float ry,
		unsigned int color, bool fill);
	void Draw3DPoint(vector pt, PPC *ppc, int psize, vector color);
	void Draw3DSegment(vector p0, vector p1, PPC *ppc, unsigned int color);
	void Convolve33(matrix kernel, FrameBuffer *& fb1);
	int handle(int);
	
	void SetZ(int u, int v, float currz);
	bool IsFarther(int u, int v, float cuurz);
	bool IsOutsideFrame(int u, int v);
	void Clear(unsigned int bgr, float z0);
	void GLFrameClear();
};