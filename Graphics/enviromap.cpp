#include "enviromap.h"
#include "scene.h"

#include <math.h>

PPC *lastCam;
FrameBuffer *lastFace;

//Initialize the enviromap at center of world facing NORTH
enviromap::enviromap(vector center, vector vN) {

	worldCenter = center;
	toNorth = vN.norm();
	toEast = toSky = toNorth;
	toEast.rotateVector(YAXIS, -90.0f);
	toSky.rotateVector(XAXIS, 90.0f);
	float wh = 512.0f;
	float hfov = 90.0f;

	faces = new FrameBuffer*[6];
	faceCams = new PPC*[6];
	for( int fi = 0; fi < 6; fi++ ) {
		faces[fi] = new FrameBuffer(0,0, wh, wh);
		faceCams[fi] = new PPC(hfov, wh, wh); 
	}
	faceCams[EM_EAST]->PositionAndOrient(worldCenter, worldCenter+toEast, YAXIS);
	faceCams[EM_WEST]->PositionAndOrient(worldCenter, worldCenter-toEast, YAXIS);
	faceCams[EM_SKY]->PositionAndOrient(worldCenter, worldCenter+toSky, ZAXIS);
	faceCams[EM_GROUND]->PositionAndOrient(worldCenter, worldCenter-toSky, -ZAXIS);
	faceCams[EM_SOUTH]->PositionAndOrient(worldCenter, worldCenter-toNorth, YAXIS);
	faceCams[EM_NORTH]->PositionAndOrient(worldCenter, worldCenter+toNorth, YAXIS);

	float cmc[72] =
		{//	 x,		y,	   z,	  x,	 y,		z,	   x,	  y,	 z,		x,	   y,	  z
			 1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f, // +X East		0 
			-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, // -X West		1
			-1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, // +Y Sky		2
			-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, // -Y Ground	3
			 1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, // +Z South		4
			-1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, // -Z North		5
		};
	CubeMapCoord = new float[72];
	for( int i = 0; i < 72; i++ ){
		CubeMapCoord[i] = cmc[i];
	}
	enable = true;
	
}

void enviromap::Render(PPC *ppc, FrameBuffer *fb) {
	for (int v = 0; v < fb->h; v++) {
		for (int u = 0; u < fb->w; u++) {
			vector camRay = ppc->GetRayForPix(u , v);
			fb->Set(u, v, getColor(camRay));
		}
	}
}

unsigned int enviromap::getColor( vector vd ) {

	vector vdn = vd.norm();
	vector colorPix;
	PPC *currCam = lastCam;
	FrameBuffer *currFace = lastFace;

	vector ip;
	/*
	if (vdn.IntersectPlaneWithRay(vdn, toEast, getCubeVertex(EM_EAST, EM_BL), ip) ) {
		float x = (ip - getCubeVertex(EM_EAST, EM_BL))*zaxis;
		float y = (ip - getCubeVertex(EM_EAST, EM_BL))*yaxis;
		if( x > 0.0f && x < 2.0f && y > 0.0f && y < 2.0f ) {
			lastCam = currCam = faceCams[EM_EAST];
			currCam->Project(vdn, colorPix);
			lastFace = currFace = faces[EM_EAST];
		}
	}
	if (vdn.IntersectPlaneWithRay(vdn, toEast, getCubeVertex(EM_WEST, EM_BL), ip) ) {
		float x = (ip - getCubeVertex(EM_WEST, EM_BL))*-zaxis;
		float y = (ip - getCubeVertex(EM_WEST, EM_BL))*yaxis;
		if( x > 0.0f && x < 2.0f && y > 0.0f && y < 2.0f ) {
			lastCam = currCam = faceCams[EM_WEST];
			currCam->Project(vdn, colorPix);
			lastFace = currFace = faces[EM_WEST];
		}
	}
	if (vdn.IntersectPlaneWithRay(vdn, toSky, getCubeVertex(EM_SKY, EM_BL), ip) ) {
		float x = (ip - getCubeVertex(EM_SKY, EM_BL))*xaxis;
		float y = (ip - getCubeVertex(EM_SKY, EM_BL))*zaxis;
		if( x > 0.0f && x < 2.0f && y > 0.0f && y < 2.0f ) {
			lastCam = currCam = faceCams[EM_SKY];
			currCam->Project(vdn, colorPix);
			lastFace = currFace = faces[EM_SKY];
		}
	}
	if (vdn.IntersectPlaneWithRay(vdn, toSky, getCubeVertex(EM_GROUND, EM_BL), ip) ) {
		float x = (ip - getCubeVertex(EM_GROUND, EM_BL))*xaxis;
		float y = (ip - getCubeVertex(EM_GROUND, EM_BL))*-zaxis;
		if( x > 0.0f && x < 2.0f && y > 0.0f && y < 2.0f ) {
			lastCam = currCam = faceCams[EM_GROUND];
			currCam->Project(vdn, colorPix);
			lastFace = currFace = faces[EM_GROUND];
		}
	}
	if (vdn.IntersectPlaneWithRay(vdn, toNorth, getCubeVertex(EM_SOUTH, EM_BL), ip) ) {
		float x = (ip - getCubeVertex(EM_SOUTH, EM_BL))*-xaxis;
		float y = (ip - getCubeVertex(EM_SOUTH, EM_BL))*yaxis;
		if( x > 0.0f && x < 2.0f && y > 0.0f && y < 2.0f ) {
			lastCam = currCam = faceCams[EM_SOUTH];
			currCam->Project(vdn, colorPix);
			lastFace = currFace = faces[EM_SOUTH];
		}
	}
	if (vdn.IntersectPlaneWithRay(vdn, toNorth, getCubeVertex(EM_NORTH, EM_BL), ip) ) {
		float x = (ip - getCubeVertex(EM_NORTH, EM_BL))*xaxis;
		float y = (ip - getCubeVertex(EM_NORTH, EM_BL))*yaxis;
		if( x > 0.0f && x < 2.0f && y > 0.0f && y < 2.0f ) {
			lastCam = currCam = faceCams[EM_NORTH];
			currCam->Project(vdn, colorPix);
			lastFace = currFace = faces[EM_NORTH];
		}
	}
	*/
	vector eastP = getCubeVertex(EM_EAST, EM_BL);
	vector westP = getCubeVertex(EM_WEST, EM_BL);
	vector skyP = getCubeVertex(EM_SKY, EM_BL);
	vector groundP = getCubeVertex(EM_GROUND, EM_BL);
	vector southP = getCubeVertex(EM_SOUTH, EM_BL);
	vector northP = getCubeVertex(EM_NORTH, EM_BL);
	if( inFace(vdn, toEast, zaxis, yaxis, eastP) ) {
		lastCam = currCam = faceCams[EM_EAST];
		currCam->Project(vdn, colorPix);
		lastFace = currFace = faces[EM_EAST];
	}
	if( inFace(vdn, -toEast, -zaxis, yaxis, westP) ) {
		lastCam = currCam = faceCams[EM_WEST];
		currCam->Project(vdn, colorPix);
		lastFace = currFace = faces[EM_WEST];
	}
	if( inFace(vdn, toSky, xaxis, zaxis, skyP) ) {
		lastCam = currCam = faceCams[EM_SKY];
		currCam->Project(vdn, colorPix);
		lastFace = currFace = faces[EM_SKY];
	}
	if( inFace(vdn, -toSky, xaxis, -zaxis, groundP) ) {
		lastCam = currCam = faceCams[EM_GROUND];
		currCam->Project(vdn, colorPix);
		lastFace = currFace = faces[EM_GROUND];
	}
	if( inFace(vdn, toNorth, -xaxis, yaxis, southP) ) {
		lastCam = currCam = faceCams[EM_SOUTH];
		currCam->Project(vdn, colorPix);
		lastFace = currFace = faces[EM_SOUTH];
	}
	if( inFace(vdn, toNorth, xaxis, yaxis, northP) ) {
		lastCam = currCam = faceCams[EM_NORTH];
		currCam->Project(vdn, colorPix);
		lastFace = currFace = faces[EM_NORTH];
	}
	//cout << colorPix << endl;
	if( currFace->IsOutsideFrame(colorPix[0], colorPix[1]) )
		return 0;
	return currFace->Get(colorPix[0], colorPix[1]);

}

bool enviromap::inFace(vector vdn, vector faceN, vector xD, vector yD, vector facePoint) {
	vector ip;
	if (vdn.IntersectPlaneWithRay(vdn, faceN, facePoint, ip) ) {
			float x = (ip - facePoint)*xD;
			float y = (ip - facePoint)*yD;
			if( x > 0.0f && x < 2.0f && y > 0.0f && y < 2.0f ) 
				return true;
	}
	return false;
}

vector enviromap::getCubeVertex(int face, int corner) {
	vector ret;
	ret = vector(CubeMapCoord[face*12+(corner+0)], 
				 CubeMapCoord[face*12+(corner+1)],	
				 CubeMapCoord[face*12+(corner+2)]);
	return ret;
}

bool enviromap::save() {

	string path = "enviromap/";
	string imageName[6] = {"east.tif", "west.tif", "sky.tif", 
						   "ground.tif","south.tif", "north.tif"};

	int w = faces[0]->w;
	int h = faces[0]->h;
	for( int fi = 0; fi < 6; fi++ ) {
		imageName[fi] = path+imageName[fi];
		int samplerperpixel = 4;
		char *image = new char[w*h*samplerperpixel];
		image = (char*)faces[fi]->pix;
		TIFF* tif = TIFFOpen(imageName[fi].c_str(), "w");
		TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, w);
		TIFFSetField(tif, TIFFTAG_IMAGELENGTH, h);
		TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
		TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplerperpixel);
		TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8); 
		TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
		TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

		tsize_t linebytes = samplerperpixel * w;

		unsigned char* line = NULL;
		if(TIFFScanlineSize(tif) == linebytes) 
			line = (unsigned char *)_TIFFmalloc(linebytes);
		else
			line = (unsigned char *)_TIFFmalloc(TIFFScanlineSize(tif));
		TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(tif, w*samplerperpixel));

		for(int i = 0; i < h; i++) {
			memcpy(line, &(image)[(h-i-1)*linebytes], linebytes);
			if(TIFFWriteScanline(tif, line, i, 0) < 0)
				break;
		}
		TIFFClose(tif);
		if(line)_TIFFfree(line);
		cout << imageName[fi] << " saved." << endl;
	}
	return true;
}

bool enviromap::open() {

	string path = "enviromap/";
	string imageName[6] = {"north.tif", "west.tif", "south.tif", 
						  "east.tif", "sky.tif", "ground.tif"};

	int w = faces[0]->w;
	int h = faces[0]->h;
	for( int fi = 0; fi < 6; fi++ ) {
		imageName[fi] = path + imageName[fi];
		TIFF* tif = TIFFOpen(imageName[fi].c_str(), "r");
		if (tif) {
			uint32 w,h;

			TIFFGetField( tif, TIFFTAG_IMAGEWIDTH, &w );
			TIFFGetField( tif, TIFFTAG_IMAGELENGTH, &h );

			TIFFReadRGBAImageOriented( tif, w, h, faces[fi]->pix, ORIENTATION_BOTLEFT, 0 );
			TIFFClose(tif);
		}
	}
	return true;
}


