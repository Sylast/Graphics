#include "enviromap.h"

#include "scene.h"

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
	faceCams[EM_NORTH]->PositionAndOrient(worldCenter, worldCenter+toNorth, YAXIS);
	faceCams[EM_WEST]->PositionAndOrient(worldCenter, worldCenter-toEast, YAXIS);
	faceCams[EM_SOUTH]->PositionAndOrient(worldCenter, worldCenter-toNorth, YAXIS);
	faceCams[EM_EAST]->PositionAndOrient(worldCenter, worldCenter+toEast, YAXIS);
	faceCams[EM_SKY]->PositionAndOrient(worldCenter, worldCenter+toSky, ZAXIS);
	faceCams[EM_GROUND]->PositionAndOrient(worldCenter, worldCenter-toSky, -ZAXIS);

	Render();
	
	enable = true;
	
}

unsigned int enviromap::getColor( vector vd ) {

	vector vdn = vd.norm();
	vector colorPix;
	PPC *currFaceCam;
	FrameBuffer *currFace;
	if( faceCams[EM_NORTH]->Project(vdn, colorPix) ){
		currFace = faces[EM_NORTH];	
		currFaceCam = faceCams[EM_NORTH];	
	}else if( faceCams[EM_WEST]->Project(vdn, colorPix) ){
		currFace = faces[EM_WEST];	
		currFaceCam = faceCams[EM_WEST];	
	}else if( faceCams[EM_SOUTH]->Project(vdn, colorPix) ){
		currFace = faces[EM_SOUTH];	
		currFaceCam = faceCams[EM_SOUTH];	
	}else if( faceCams[EM_EAST]->Project(vdn, colorPix) ){
		currFace = faces[EM_EAST];	
		currFaceCam = faceCams[EM_EAST];	
	}else if( faceCams[EM_SKY]->Project(vdn, colorPix) ){
		currFace = faces[EM_SKY];	
		currFaceCam = faceCams[EM_SKY];	
	}else if( faceCams[EM_GROUND]->Project(vdn, colorPix) ){
		currFace = faces[EM_GROUND];	
		currFaceCam = faceCams[EM_GROUND];	
	}

	cout << colorPix << endl;
	return currFace->Get(colorPix[0], colorPix[1]);

}

void enviromap::Render() {

	for( int fbi = 0; fbi < 6; fbi++ ) {
		scene->tmeshes[3]->RenderFilled(faceCams[fbi], faces[fbi], BLACK, scene->lightsN,
										  scene->lights, scene->ka, 
										  scene->textures[scene->tmeshes[3]->texIndex],
										  scene->tmeshes[3]->RenderMode);
	}

	//Set up to only render meshes that are faraway. for now just single one
	/*
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
		scene->tmeshes[3]->RenderFilled(ppc, faces[3], BLACK, scene->lightsN,
										  scene->lights, scene->ka, 
										  scene->textures[scene->tmeshes[3]->texIndex],
										  scene->tmeshes[3]->RenderMode);
	}
	*/

}

bool enviromap::save() {

	string path = "enviromap/";
	string imageName[6] = {"north.tif", "west.tif", "south.tif", 
						  "east.tif", "sky.tif", "ground.tif"};

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


