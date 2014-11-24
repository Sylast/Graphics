#pragma once


#include "framebuffer.h"
#include "ppc.h"
#include "vector.h"
#define PATH 

class enviromap
{
	FrameBuffer **faces;
	PPC *ppc;
	vector world;
	bool enable;
	
public:
	enviromap() : faces((0)), ppc(0), enable(false){};
	enviromap(vector center, vector direction);

	unsigned int getColor( vector vd );
	void Render();
	bool save();
	bool open();
};

