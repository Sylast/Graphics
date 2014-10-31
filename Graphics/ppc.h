#pragma once

#include "vector.h"

#define PPC_UP			0
#define	PPC_DOWN		1
#define PPC_LEFT		2
#define PPC_RIGHT		3
#define	PPC_FORWARD		4
#define	PPC_BACKWARD	5

#define PPC_ROLL	0
#define PPC_TILT	1
#define PPC_PAN		2

class FrameBuffer;

class PPC {
public:
	vector a, b, c, C;
	int w, h;
	PPC(float hfov, int w, int h);
	bool Project(vector P, vector &PP);
	vector UnProject(vector pp);

	void PositionAndOrient(vector newC, vector lap, vector vpv);

	float GetF();
	vector GetVD();
	vector GetRayForPix(int u, int v);
	vector GetRayForPIP(float uf, float vf);
	vector GetPC(int u, int v);
	float Gethfov();
	vector GetPP();
	vector GetPointOnFocalPlane(float uf, float vf, float f);

	void translate(int direction, float sf);
	void rotate(int type, float alpha);
	void zoom(float sf);

	void position(vector lap, float d, vector vd);
	PPC* interpolate(PPC* newCamera, vector subject, float f);
	void SetByInterpolation(PPC *cam0, PPC* cam1, float f);
	void RenderWireframe(PPC *ppc, FrameBuffer *fb, float f, unsigned int color);

	void SetIntrinsics(float zNear, float zFar);
	void SetExtrinsics();
};
