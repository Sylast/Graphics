#pragma once


#include <math.h>
#include "framebuffer.h"
#include "ppc.h"
#include "vector.h"

#define EM_NORTH	0
#define EM_WEST		1
#define EM_SOUTH	2
#define EM_EAST		3
#define EM_SKY		4
#define EM_GROUND	5

class enviromap
{
	FrameBuffer **faces;
	PPC **faceCams;
	vector worldCenter;
	vector toNorth, toEast, toSky;
	bool enable;
	
public:
	enviromap(vector center = ORIGIN, vector direction = -ZAXIS);

	unsigned int getColor( vector vd );
	void Render();
	bool save();
	bool open();
};

