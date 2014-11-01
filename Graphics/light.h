#pragma once

#include "vector.h"
#include "ppc.h"
#include "framebuffer.h"

class Light
{
public:
	PPC *L;
	FrameBuffer *sm;
	bool on;

	Light() : L(0), sm(0), on(false){};
	Light(vector center, vector lap, int w, int h);

};