#include "enviromap.h"

#include "scene.h"

//Initialize the enviromap at center of world facing NORTH
enviromap::enviromap(vector center, vector toNorth) {

	world = center;
	float wh = 512.0f;
	float hfov = 90.0f;
	ppc = new PPC(hfov, wh, wh); 
	ppc->PositionAndOrient(world, world+toNorth, YAXIS);
	
	
}

unsigned int enviromap::getColor( vector vd ) {

}

void enviromap::Render() {

	float MAPDIS = 1000.0f;

	for (int tmi = 0; tmi < scene->tmeshesN; tmi++) {
		if (!scene->tmeshes[tmi]->enabled)
			continue;
		float dis = (scene->tmeshes[tmi]->GetCenter() - world).length();
		if (dis < MAPDIS)
			continue;
		for (int fbi = 0; fbi < 6; fbi++) {
			scene->tmeshes[tmi]->RenderFilled(ppc, faces[fbi], BLACK, scene->lightsN,
											  scene->lights, scene->ka, 
											  scene->textures[scene->tmeshes[tmi]->texIndex],
											  scene->tmeshes[tmi]->RenderMode);
		}
	}

}

bool enviromap::save() {

}

bool enviromap::open() {

}


