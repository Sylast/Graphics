#include "scene.h"
#include "matrix.h"

#include <iostream>

using namespace std;

Scene *scene;

Scene::Scene() {

	gui = new GUI();
	gui->show();

	HWinit = false;

	int u0 = 280;
	int v0 = 50;
	int sci = 2;
	int w = sci*320;
	int h = sci*240;
	fb = new FrameBuffer(u0, v0, w, h);
	fb->label("SW Framebuffer");
	fb->show();
	hwfb = new FrameBuffer(u0+w+20, v0, w, h);
	hwfb->label("HW Framebuffer");
	hwfb->show();
	hwfb->isHW = true;
	gui->uiw->position(20, 50);

	float hfov = 55.0f;
	ppc = new PPC(hfov, w, h);

	texturesN = 1;
	textures = new FrameBuffer*[texturesN];
	texName = new GLuint[texturesN];
	char * name = "TIFFimages/Brick_Wall_Texture.tif";
	textures[0] = new FrameBuffer(0,0,512,512);
	openTexture(name, textures[0]);

	tmeshesN = 4;
	tmeshes = new TMesh*[tmeshesN];

	vector center(0.0f, 0.0f, 0.0f);
	vector dims(1000.0f, 25.0f, 1000.0f);
	unsigned int color = RED;
	tmeshes[0] = new TMesh(center, dims, color, 0, 5.0);
	tmeshes[0]->Position(vector(0.0f, -50.0f, -600.0f));

	tmeshes[1] = new TMesh();
	tmeshes[1]->LoadBin("geometry/teapot1k.bin");
	vector newCenter = vector(-75.0f, -10.0f, -300.0f);
	tmeshes[1]->Position(newCenter);
	tmeshes[1]->RenderMode = MCI;
	
	tmeshes[2] = new TMesh();
	tmeshes[2]->LoadBin("geometry/teapot1K.bin");
	tmeshes[2]->RenderMode = WF;
	newCenter = vector(75.0f, 3.0f, -300.0f);
	tmeshes[2]->Scale(1.5f);
	tmeshes[2]->Position(newCenter);
	tmeshes[2]->Rotate(yaxis, 50.0f);
	
	tmeshes[3] = new TMesh();
	tmeshes[3]->LoadBin("geometry/teapot1K.bin");
	tmeshes[3]->RenderMode = MCI;
	newCenter = vector(0.0f, 30.0f, -150.0f);
	tmeshes[3]->Scale(0.5f);
	tmeshes[3]->Position(newCenter);
	tmeshes[3]->Rotate(xaxis, 90.0f);
	
	//tmeshes[0]->enabled = false;
	//tmeshes[1]->enabled = false;
	//tmeshes[2]->enabled = false;
	tmeshes[3]->enabled = false;

	lightsN = 2;
	ka = 0.1f;
	lights = new Light*[lightsN];
	vector lp(300.0f, 50.0f, -300.0f);
	lights[0] = new Light(lp, tmeshes[1]->GetCenter(), 512, 512);
	lp = vector(-100.0f, 50.0f, -700.0f);
	lights[1] = new Light(lp, tmeshes[1]->GetCenter(), 512, 512);
	//lights[1]->on = false;
	//lights[0]->on = false;

	RenderShadow();
	Render();

}

void Scene::Render() {

	if(hwfb)
		hwfb->redraw();

	fb->Clear(0xFFAAAAAA, 0.0f);
	for (int tmi = 0; tmi < tmeshesN; tmi++) {
		if (!tmeshes[tmi]->enabled)
			continue;
		//tmeshes[tmi]->RenderPoints(ppc, fb, 3);

		if(tmeshes[tmi]->RenderMode == WF)
			tmeshes[tmi]->RenderWireframe(ppc, fb, BLACK);
		else
			tmeshes[tmi]->RenderFilled(ppc, fb, RED, lightsN, lights, ka, textures[tmeshes[tmi]->texIndex], tmeshes[tmi]->RenderMode);
	}
	//fb->Draw3DPoint(lights[0]->L->C, ppc, 10, vector(1,1,1));
	fb->redraw();

}

void Scene::RenderShadow() {

	for( int li = 0; li < lightsN; li++ ){
			if (!lights[li]->on)
				continue;
		lights[li]->sm->Clear(WHITE, 0.0f);
		for (int tmi = 0; tmi < tmeshesN; tmi++) {
			if (!tmeshes[tmi]->enabled)
				continue;
			tmeshes[tmi]->RenderFilled(lights[li]->L, lights[li]->sm, BLACK, 0, NULL, 0.0f, NULL, SM);
		}
	}
}

void Scene::RenderHW() {
	hwfb->GLFrameClear();

	float zNear = 1.0f;
	float zFar = 1000.0f;
	ppc->SetIntrinsics(zNear, zFar);
	ppc->SetExtrinsics();

	for( int tmi = 0; tmi < tmeshesN; tmi++ ){
		if( !tmeshes[tmi]->enabled )
			continue;
		tmeshes[tmi]->RenderHW();
	}
}

void Scene::InitializeTextures() {

	for(int ti = 0; ti < texturesN; ti++) {
		glGenTextures(texturesN, texName);
		glBindTexture(GL_TEXTURE_2D, texName[ti]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textures[ti]->w, textures[ti]->h,
						0, GL_RGBA, GL_UNSIGNED_BYTE, textures[ti]->pix);  
		glBindTexture(GL_TEXTURE_2D, 0);
		GLenum err;
		while((err = glGetError()) != GL_NO_ERROR) {
			cerr << "INIT" << err << endl;
		}
	}

}

void Scene::InitializeLights() {
	
	float ka[] = {0.0f, 0.0f, 0.0f, 1.0f};
	float kd[] = {1.0f, 1.0f, 1.0f, 1.0f};
	float ks[] = {1.0f, 1.0f, 1.0f, 1.0f};

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	if(!lights[0]->on)
		return;
	vector LP = lights[0]->L->C;
	vector LD = lights[0]->L->GetVD();
	float lightPos[] = {LP[0], LP[1], LP[2], 0};
	float lightDir[] = {LD[0], LD[1], LD[2], 0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDir);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ka);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, kd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, ks);
	glEnable(GL_LIGHT0);

}

void Scene::save( const char* imageName ) {
	if(imageName == NULL) return;

	int samplerperpixel = 4;
	char *image = new char[fb->w*fb->h*samplerperpixel];
	image = (char*)fb->pix;
	TIFF* tif = TIFFOpen(imageName, "w");
	TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, fb->w);
	TIFFSetField(tif, TIFFTAG_IMAGELENGTH, fb->h);
	TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplerperpixel);
	TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8); 
	TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

	tsize_t linebytes = samplerperpixel * fb->w;

	unsigned char* line = NULL;
	if(TIFFScanlineSize(tif) == linebytes) 
		line = (unsigned char *)_TIFFmalloc(linebytes);
	else
		line = (unsigned char *)_TIFFmalloc(TIFFScanlineSize(tif));
	TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(tif, fb->w*samplerperpixel));

	for(int i = 0; i < fb->h; i++) {
		memcpy(line, &(image)[(fb->h-i-1)*linebytes], linebytes);
		if(TIFFWriteScanline(tif, line, i, 0) < 0)
			break;
	}
	TIFFClose(tif);
	if(line)_TIFFfree(line);
}

void Scene::open( const char* imageName ) {
	if(imageName == NULL) return;
	TIFF* tif = TIFFOpen(imageName, "r");
	if (tif) {
		uint32 w, h;

		TIFFGetField( tif, TIFFTAG_IMAGEWIDTH, &w );
		TIFFGetField( tif, TIFFTAG_IMAGELENGTH, &h );

		delete(fb);
		fb = new FrameBuffer(280, 50, w, h);
		fb->label("Frame");
		fb->show();
		if (TIFFReadRGBAImageOriented( tif, w, h, fb->pix, ORIENTATION_BOTLEFT, 0 )) {
			fb->redraw();
		}
		TIFFClose(tif);
	}

}
void Scene::openTexture( const char* imageName, FrameBuffer *texture ) {
	if(imageName == NULL) return;
	TIFF* tif = TIFFOpen(imageName, "r");
	if (tif) {
		uint32 w,h;

		TIFFGetField( tif, TIFFTAG_IMAGEWIDTH, &w );
		TIFFGetField( tif, TIFFTAG_IMAGELENGTH, &h );

		TIFFReadRGBAImageOriented( tif, w, h, texture->pix, ORIENTATION_BOTLEFT, 0 );
		TIFFClose(tif);
	}

}

void Scene::blur() {

	matrix blurk( 
		vector(0.0f, 1.0f, 0.0f),
		vector(1.0f, 4.0f, 1.0f),
		vector(0.0f, 1.0f, 0.0f) 
		);

	blurk.normalize();

	fb->Convolve33(blurk, fb);
	fb->redraw();
	return;
}

void Scene::edge() {

	matrix edgek( 
		vector(-1.0f, -1.0f, -1.0f),
		vector(-1.0f, 8.0f, -1.0f),
		vector(-1.0f, -1.0f, -1.0f) 
		);

	fb->Convolve33(edgek, fb);
	fb->redraw();
	return;
}

void Scene::brightness( float value ) {
	value /= 100;

	vector newColor(0.0f, 0.0f, 0.0f);
	for( int v = 0; v < fb->h; v++ ) {
		for( int u = 0; u < fb->w; u++ ) {
			vector currColor;
			currColor.SetFromColor( fb->Get(u,v) );
			newColor = currColor + currColor * value;
			fb->Set( u, v, newColor.GetColor() );
		}
	}
	fb->redraw();
}
void Scene::contrast( float value ) {

	vector newColor(0.0f, 0.0f, 0.0f);
	for( int v = 0; v < fb->h; v++ ) {
		for( int u = 0; u < fb->w; u++ ) {
			vector currColor;
			currColor.SetFromColor( fb->Get(u,v) );
			newColor = currColor * value;
			fb->Set( u, v, newColor.GetColor() );
		}
	}
	fb->redraw();
}

void Scene::rasterShape( int x0, int y0, int x1, int y1, unsigned int) {
	unsigned int color = gui->rgb->color();
	unsigned int red = ((unsigned char*)&color)[3];
	unsigned int green = ((unsigned char*)&color)[2];
	unsigned int blue = ((unsigned char*)&color)[1];
	color = 0xFF000000 + blue * 256*256 + green * 256 + red;
	vector zero( x0, y0, 0 );
	vector one( x1, y1, 0 );
	if(gui->l->value()) {
		fb->DrawSegment(zero, one, color);
		fb->redraw();
	}
	if(gui->c->value()) {
		fb->DrawCircle((float)x0, (float)y0, (float)x1, (float)y1, color, gui->f->value());
		fb->redraw();
	}

}

int xOld, yOld;
int FrameBuffer::handle(int event) {
	unsigned int color = BLACK;
	switch(event) {
	case FL_PUSH:
		xOld = Fl::event_x();	
		yOld = Fl::event_y();	
		return 1;

	case FL_RELEASE:
		scene->rasterShape( xOld, yOld, Fl::event_x(), Fl::event_y(), color );

		return 1;
	case FL_KEYDOWN:
		if (Fl::event_key(FL_Up)) {
			scene->ppc->translate(PPC_UP, 10);
			scene->Render();
		}else if(Fl::event_key(FL_Down)) {
			scene->ppc->translate(PPC_DOWN, 10);
			scene->Render();
		}else if(Fl::event_key(FL_Left)) {
			scene->ppc->translate(PPC_LEFT, 10);
			scene->Render();
		}else if(Fl::event_key(FL_Right)) {
			scene->ppc->translate(PPC_RIGHT, 10);
			scene->Render();
		}else if(Fl::event_key(FL_Page_Up)) {
			scene->ppc->translate(PPC_FORWARD, 10);
			scene->Render();
		}else if(Fl::event_key(FL_Page_Down)) {
			scene->ppc->translate(PPC_BACKWARD, 10);
			scene->Render();
		}else if(Fl::event_key('a')) {
			scene->ppc->rotate(PPC_PAN, 1);
			scene->Render();
		}else if(Fl::event_key('d')) {
			scene->ppc->rotate(PPC_PAN, -1);
			scene->Render();
		}else if(Fl::event_key('w')) {
			scene->ppc->rotate(PPC_TILT, -1);
			scene->Render();
		}else if(Fl::event_key('s')) {
			scene->ppc->rotate(PPC_TILT, 1);
			scene->Render();
		}else if(Fl::event_key('q')) {
			scene->ppc->rotate(PPC_ROLL, -1);
			scene->Render();
		}else if(Fl::event_key('e')) {
			scene->ppc->rotate(PPC_ROLL, 1);
			scene->Render();
		}else if(Fl::event_key('i')) {
			scene->ppc->zoom(.1);
			scene->Render();
		}else if(Fl::event_key('o')) {
			scene->ppc->zoom(-.1);
			scene->Render();
		}

		return 1;
	}
}

void Scene::DBG() {

	int save = 0;
	string file = "Video/img";
	file += to_string(save);
	file += ".tif";
	scene->save(file.c_str());
	file = "Video/img";
	file += to_string(save++);
	file += ".tif";
	{
	PPC *ppc0 = new PPC(*ppc);
	PPC *ppc1 = new PPC(*ppc);
	ppc1->C = ppc1->C + vector(70.0f, 30.0f, 10.0f);
	vector lap = (tmeshes[1]->GetCenter() + tmeshes[2]->GetCenter())/2;
	ppc1->PositionAndOrient(ppc1->C, lap, vector(0.0f, 1.0f, 0.0f));
    int stepsN = 50;
    for (int si = 0; si < stepsN; si++) {
      ppc->SetByInterpolation(ppc0, ppc1, (float) si / (float) (stepsN-1));
      Render();
      Fl::check();
		scene->save(file.c_str());
		file = "Video/img";
		file += to_string(save++);
		file += ".tif";
    }
	}

	{
    PPC ppc1(*(lights[0]->L));
	vector newC(500.0f, 0.0f, -300.0f);
    vector lap = tmeshes[1]->GetCenter();
    vector vpv(0.0f, 1.0f, 0.0f);
    ppc1.PositionAndOrient(newC, lap, vpv);
    float f = 40.0f;

    PPC ppc2(*(lights[0]->L));
	vector newC2(300.0f, 50.0f, -200.0f);
    vector lap2 = tmeshes[1]->GetCenter();
    vector vpv2(0.0f, 1.0f, 0.0f);
    ppc2.PositionAndOrient(newC2, lap2, vpv2);

    int stepsN = 50;
    for (int si = 0; si < stepsN; si++) {
      fb->Clear(0xFFFFFF, 0.0f);

      PPC ppc3(*ppc);
      lights[0]->L->SetByInterpolation(&ppc1, &ppc2, (float) si / (float) (stepsN-1));
	  RenderShadow();
	  Render();
      Fl::check();
		scene->save(file.c_str());
		file = "Video/img";
		file += to_string(save++);
		file += ".tif";
    }
	{
	vector center = tmeshes[2]->GetCenter();
	vector newPos = center + vector(-30.0f, 0.0f, -50.0f);
    int stepsN = 50;
    for (int si = 0; si < stepsN; si++) {
      fb->Clear(0xFFFFFF, 0.0f);
	  vector pos = center + (newPos - center) * (float)si / (float) (stepsN-1);
	  tmeshes[2]->Position(pos);
	  RenderShadow();
	  Render();
      Fl::check();
		scene->save(file.c_str());
		file = "Video/img";
		file += to_string(save++);
		file += ".tif";
    }

	}

    fb->redraw();
    return;
  }
    return;

	return;


	ppc->zoom(2);
	Render();
	return;

	for(int i = 0; i < 36; i++) {
		ppc->rotate(PPC_PAN, 10);
		Render();
		Fl::check();
	}
	return;


	vector C0 = ppc->C;
	vector C1(0.0f, 180.0f, -30.0f);
	vector lap = tmeshes[1]->GetCenter();
	vector vpv = vector(0.0f, 1.0f, 0.0f);
	int stepsN = 100;
	for (int si = 0; si < stepsN; si++) {
		vector newC = C0 + (C1-C0)*(float)si/(float)(stepsN-1);
		ppc->PositionAndOrient(newC, lap, vpv);
		Render();
		Fl::check();
	}

	return;
}