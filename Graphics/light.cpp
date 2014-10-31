#include "light.h"


Light::Light(vector center, vector lap, int w, int h) {
	
	float hfov = 55.0f;
	L = new PPC(hfov, w, h);
	L->PositionAndOrient(center, lap, vector(0.0f, 1.0f, 0.0f));
	on = true;
	
	sm = new FrameBuffer(0,0, w, h);
	sm->Clear(0xFFFFFFFF, 0.0f);
}
