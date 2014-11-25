#pragma once

#include "framebuffer.h"
#include "ppc.h"
#include "vector.h"

#define EM_EAST		0 //+X
#define EM_WEST		1 //-X
#define EM_SKY		2 //+Y
#define EM_GROUND	3 //-Y
#define EM_SOUTH	4 //+Z
#define EM_NORTH	5 //-Z
#define EM_BL	0 //Bottom Left
#define EM_BR	1 //Bottom Right
#define EM_TR	2 //Top Right
#define EM_TL	3 //Top Left

class enviromap {
public:
	FrameBuffer **faces;
	PPC **faceCams;
	vector worldCenter;
	vector toNorth, toEast, toSky;
	float *CubeMapCoord;
	bool enable;
	
	enviromap(vector center = ORIGIN, vector direction = -ZAXIS);

	void Render(PPC *ppc, FrameBuffer *fb);
	unsigned int getColor( vector vd );
	bool inFace(vector vdn, vector faceN, vector xD, vector yD, vector facePoint);
	vector getCubeVertex(int face, int corner);
	bool save();
	bool open();
};

