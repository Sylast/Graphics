#include "tmesh.h"
#include "scene.h"

#include <fstream>
#include <iostream>


TMesh::TMesh(vector center, vector dims, unsigned int color) {

	enabled = true;
	shade = false;
	vertsN = 8;
	verts = new vector[vertsN];
	cols = new vector[vertsN];
	normals = 0;

	int vi = 0;
	verts[vi++] = center + vector(-dims[0]/2.0f, +dims[1]/2.0f, +dims[2]/2.0f);
	verts[vi++] = center + vector(-dims[0]/2.0f, -dims[1]/2.0f, +dims[2]/2.0f);
	verts[vi++] = center + vector(+dims[0]/2.0f, -dims[1]/2.0f, +dims[2]/2.0f);
	verts[vi++] = center + vector(+dims[0]/2.0f, +dims[1]/2.0f, +dims[2]/2.0f);

	verts[vi++] = center + vector(-dims[0]/2.0f, +dims[1]/2.0f, -dims[2]/2.0f);
	verts[vi++] = center + vector(-dims[0]/2.0f, -dims[1]/2.0f, -dims[2]/2.0f);
	verts[vi++] = center + vector(+dims[0]/2.0f, -dims[1]/2.0f, -dims[2]/2.0f);
	verts[vi++] = center + vector(+dims[0]/2.0f, +dims[1]/2.0f, -dims[2]/2.0f);

	for (int vi = 0; vi < vertsN; vi++) {
		cols[vi].SetFromColor(color);
	}

	trisN = 12;
	tris = new unsigned int[trisN * 3];
	int tri = 0;

	tris[3*tri+0] = 0;
	tris[3*tri+1] = 1;
	tris[3*tri+2] = 2;
	tri++;
	tris[3*tri+0] = 2;
	tris[3*tri+1] = 3;
	tris[3*tri+2] = 0;
	tri++;
	tris[3*tri+0] = 3;
	tris[3*tri+1] = 2;
	tris[3*tri+2] = 6;
	tri++;
	tris[3*tri+0] = 6;
	tris[3*tri+1] = 7;
	tris[3*tri+2] = 3;
	tri++;
	tris[3*tri+0] = 4;
	tris[3*tri+1] = 0;
	tris[3*tri+2] = 3;
	tri++;
	tris[3*tri+0] = 3;
	tris[3*tri+1] = 7;
	tris[3*tri+2] = 4;
	tri++;

	tris[3*tri+0] = 5;
	tris[3*tri+1] = 6;
	tris[3*tri+2] = 2;
	tri++;
	tris[3*tri+0] = 2;
	tris[3*tri+1] = 1;
	tris[3*tri+2] = 5;
	tri++;

	tris[3*tri+0] = 4;
	tris[3*tri+1] = 7;
	tris[3*tri+2] = 6;
	tri++;
	tris[3*tri+0] = 6;
	tris[3*tri+1] = 5;
	tris[3*tri+2] = 4;
	tri++;

	tris[3*tri+0] = 4;
	tris[3*tri+1] = 5;
	tris[3*tri+2] = 1;
	tri++;
	tris[3*tri+0] = 1;
	tris[3*tri+1] = 0;
	tris[3*tri+2] = 4;
	tri++;

}

//Sets up a 3d quad with a texture and texture coord if given
TMesh::TMesh(vector center, vector dims, unsigned int color, int texN, float tile) : aabb(0) {

	enabled = true;
	shade = false;
	vertsN = 8*3;
	trisN = 12;
	verts = new vector[vertsN];
	cols = new vector[vertsN];
	normals = new vector[vertsN];
	tris = new unsigned int[trisN*3];
	tcs = new float[(vertsN)*2];

	for ( int i = 0; i < 3; i++ ) {
		int vi = 0;
		verts[(3*vi++)+i] = center + vector(-dims[0]/2.0f, +dims[1]/2.0f, +dims[2]/2.0f);
		verts[(3*vi++)+i] = center + vector(-dims[0]/2.0f, -dims[1]/2.0f, +dims[2]/2.0f);
		verts[(3*vi++)+i] = center + vector(+dims[0]/2.0f, +dims[1]/2.0f, +dims[2]/2.0f);
		verts[(3*vi++)+i] = center + vector(+dims[0]/2.0f, -dims[1]/2.0f, +dims[2]/2.0f);

		verts[(3*vi++)+i] = center + vector(+dims[0]/2.0f, +dims[1]/2.0f, -dims[2]/2.0f);
		verts[(3*vi++)+i] = center + vector(+dims[0]/2.0f, -dims[1]/2.0f, -dims[2]/2.0f);
		verts[(3*vi++)+i] = center + vector(-dims[0]/2.0f, +dims[1]/2.0f, -dims[2]/2.0f);
		verts[(3*vi++)+i] = center + vector(-dims[0]/2.0f, -dims[1]/2.0f, -dims[2]/2.0f);
	}

	for ( int vi = 0; vi < vertsN; vi++ ) {
		cols[vi].SetFromColor(color);
	}
	
	normals[0] = normals[3] = normals[7] = normals[10] = 
		vector(0.0f, 0.0f, -1.0f);
	normals[6] = normals[9] = normals[13] = normals[16] = 
		vector(1.0f, 0.0f, 0.0f);
	normals[12] = normals[15] = normals[19] = normals[22] = 
		vector(0.0f, 0.0f, 1.0f);
	normals[18] = normals[21] = normals[1] = normals[4] = 
		vector(-1.0f, 0.0f, 0.0f);
	normals[20] = normals[2] = normals[14] = normals[8] = 
		vector(0.0f, 1.0f, 0.0f);
	normals[11] = normals[17] = normals[5] = normals[23] = 
		vector(0.0f, -1.0f, 0.0f);


	int tri = 0;
	tris[3*tri+0] = 7; //2
	tris[3*tri+1] = 0; //0
	tris[3*tri+2] = 3; //1
	tri++;
	tris[3*tri+0] = 3; //1
	tris[3*tri+1] = 10; //3
	tris[3*tri+2] = 7; //2
	tri++;
	tris[3*tri+0] = 13; //4
	tris[3*tri+1] = 6; //2
	tris[3*tri+2] = 9; //3
	tri++;
	tris[3*tri+0] = 9; //3
	tris[3*tri+1] = 16; //5
	tris[3*tri+2] = 13; //4
	tri++;
	tris[3*tri+0] = 19; //6
	tris[3*tri+1] = 12; //4
	tris[3*tri+2] = 15; //5
	tri++;
	tris[3*tri+0] = 15; //5
	tris[3*tri+1] = 22; //7
	tris[3*tri+2] = 19; //6
	tri++;
	tris[3*tri+0] = 1; //0
	tris[3*tri+1] = 18; //6
	tris[3*tri+2] = 21; //7
	tri++;
	tris[3*tri+0] = 21;//7
	tris[3*tri+1] = 4; //1
	tris[3*tri+2] = 1; //0
	tri++;
	tris[3*tri+0] = 20; //6
	tris[3*tri+1] = 2; //0
	tris[3*tri+2] = 8; //2
	tri++;
	tris[3*tri+0] = 8; //2
	tris[3*tri+1] = 14; //4
	tris[3*tri+2] = 20;//6
	tri++;
	tris[3*tri+0] = 11; //3
	tris[3*tri+1] = 5; //1
	tris[3*tri+2] = 23; //7
	tri++;
	tris[3*tri+0] = 23; //7
	tris[3*tri+1] = 17; //5
	tris[3*tri+2] = 11; //3

	int tci = 0;
	//Hard coded in the texture as 2* vert +0|1
	tcs[2*0] = 0.0f;
	tcs[2*0+1] = 0.0f;
	tcs[2*3] = tile;
	tcs[2*3+1] = 0.0f;
	tcs[2*7] = 0.0f;
	tcs[2*7+1] = tile;
	tcs[2*10] = tile;
	tcs[2*10+1] = tile;

	tcs[2*6] = 0.0f;
	tcs[2*6+1] = 0.0f;
	tcs[2*9] = tile;
	tcs[2*9+1] = 0.0f;
	tcs[2*13] = 0.0f;
	tcs[2*13+1] = tile;
	tcs[2*16] = tile;
	tcs[2*16+1] = tile;

	tcs[2*12] = 0.0f;
	tcs[2*12+1] = 0.0f;
	tcs[2*15] = tile;
	tcs[2*15+1] = 0.0f;
	tcs[2*19] = 0.0f;
	tcs[2*19+1] = tile;
	tcs[2*22] = tile;
	tcs[2*22+1] = tile;

	tcs[2*18] = 0.0f;
	tcs[2*18+1] = 0.0f;
	tcs[2*21] = tile;
	tcs[2*21+1] = 0.0f;
	tcs[2*1] = 0.0f;
	tcs[2*1+1] = tile;
	tcs[2*4] = tile;
	tcs[2*4+1] = tile;

	tcs[2*20] = 0.0f;
	tcs[2*20+1] = 0.0f;
	tcs[2*2] = tile;
	tcs[2*2+1] = 0.0f;
	tcs[2*14] = 0.0f;
	tcs[2*14+1] = tile;
	tcs[2*8] = tile;
	tcs[2*8+1] = tile;

	tcs[2*11] = 0.0f;
	tcs[2*11+1] = 0.0f;
	tcs[2*17] = tile;
	tcs[2*17+1] = 0.0f;
	tcs[2*5] = 0.0f;
	tcs[2*5+1] = tile;
	tcs[2*23] = tile;
	tcs[2*23+1] = tile;

	SetAABB();

	RenderMode = MCI;
	if(texN == -1) 
		return; 
	texIndex = texN;
	RenderMode = TM;
}

void TMesh::RenderPoints(PPC *ppc, FrameBuffer *fb, int psize) {

	for (int vi = 0; vi < vertsN; vi++) {
		fb->Draw3DPoint(verts[vi], ppc, psize, cols[vi]);
	}

}

void TMesh::RenderWireframe(PPC *ppc, FrameBuffer *fb, unsigned int color) {

	for (int tri = 0; tri < trisN; tri++) {
		int vinds[3];
		vinds[0] = tris[tri*3+0];
		vinds[1] = tris[tri*3+1];
		vinds[2] = tris[tri*3+2];
		for (int vi = 0; vi < 3; vi++) {
			fb->Draw3DSegment(verts[vinds[vi]], verts[vinds[(vi+1)%3]], ppc, color);
		}
	}

}
/*	Render Modes are
 *	-Vertex Color Interpolation
 *  -Texture Mapping
 *	Renders the triangle mess, shades using the sence lights.
 *  Shadow Mapping implimented 
 */
void TMesh::RenderFilled(PPC *ppc, FrameBuffer *fb, unsigned int color, int lightsN, Light **L, float ka, FrameBuffer *texture, int renderMode) {

	//Project vertices on to the camera
	vector *pverts = new vector[vertsN];
	for (int vi = 0; vi < vertsN; vi++) {
		ppc->Project(verts[vi], pverts[vi]);
	}

	for (int tri = 0; tri < trisN; tri++) {
		int vinds[3];
		vinds[0] = tris[tri*3+0];
		vinds[1] = tris[tri*3+1];
		vinds[2] = tris[tri*3+2];

		if( pverts[vinds[0]][2] == FLT_MAX || 
			pverts[vinds[1]][2] == FLT_MAX || 
			pverts[vinds[2]][2] == FLT_MAX )
			continue;

		//Compute bounding box aabb of projected vertices
		AABB aabb(pverts[vinds[0]]);
		aabb.AddPoint(pverts[vinds[1]]);
		aabb.AddPoint(pverts[vinds[2]]);

		//Clip aabb with frame
		if(!aabb.Clip(0.0f, (float) fb->w, 0.0f, (float) fb->h)) 
			continue;

		int left = (int)(aabb.corners[0][0] + 0.5f);
		int right = (int)(aabb.corners[1][0] - 0.5f);
		int top = (int)(aabb.corners[0][1] + 0.5f);
		int bottom = (int)(aabb.corners[1][1] - 0.5f);

		vector red( cols[vinds[0]][0], cols[vinds[1]][0], cols[vinds[2]][0]); 
		vector green( cols[vinds[0]][1], cols[vinds[1]][1], cols[vinds[2]][1]); 
		vector blue( cols[vinds[0]][2], cols[vinds[1]][2], cols[vinds[2]][2]); 
		vector depth( pverts[vinds[0]][2], pverts[vinds[1]][2], pverts[vinds[2]][2] );

		vector rABC, gABC, bABC, zABC, nxABC, nyABC, nzABC, sABC, tABC;
		vector DEF;

		vector eeqs[3];
		SetEEQS( pverts[vinds[0]], pverts[vinds[1]], pverts[vinds[2]], eeqs);

		matrix ptm(pverts[vinds[0]], pverts[vinds[1]], pverts[vinds[2]]);
		ptm.setCol(2, vector(1.0f, 1.0f, 1.0f));
		ptm = invert(ptm);

		// Set up for normals
		vector nx( normals[vinds[0]][0], normals[vinds[1]][0], normals[vinds[2]][0] );
		vector ny( normals[vinds[0]][1], normals[vinds[1]][1], normals[vinds[2]][1] );
		vector nz( normals[vinds[0]][2], normals[vinds[1]][2], normals[vinds[2]][2] );
		// Set depth interpolation through screen space interpolation
		zABC = ptm * depth;

		//Matrix for Model Space interpolation
		matrix Q;
		Q = ComputeRastMatrix(ppc, verts[vinds[0]], verts[vinds[1]], verts[vinds[2]]);
		DEF = Q[0] + Q[1] + Q[2];
		//Compute Model Space interpolation constants
		SetModelRast(Q, nx, &nxABC); 
		SetModelRast(Q, ny, &nyABC); 
		SetModelRast(Q, nz, &nzABC); 
		//SetModelRast(Q, depth, &zABC);

		if( renderMode == VCI ) { 
			// Setup screen space linear variation of color 
			rABC = ptm * red;
			gABC = ptm * green;
			bABC = ptm * blue;

		}else if( renderMode == MCI ) {
			// Set Color Model Space Constants
			SetModelRast(Q, red, &rABC);
			SetModelRast(Q, green, &gABC);
			SetModelRast(Q, blue, &bABC);

		}else if( renderMode == TM) {
			//Get texture model space parameters
			vector s( tcs[2*vinds[0]+0], tcs[2*vinds[1]+0], tcs[2*vinds[2]+0]); 
			vector t( tcs[2*vinds[0]+1], tcs[2*vinds[1]+1], tcs[2*vinds[2]+1]); 
			SetModelRast(Q, s, &sABC);
			SetModelRast(Q, t, &tABC);

		}

		for( int v = top; v <= bottom; v++ ){
			for ( int u = left; u <= right; u++ ) {

				if(fb->IsOutsideFrame(u,v)) 
					continue;
				vector pv((float)u+0.5f, (float)v+0.5f, 1.0f);

				//Check whether pixel is inside triangle
				if (eeqs[0]*pv < 0.0f || eeqs[1]*pv < 0.0f || eeqs[2]*pv < 0.0f)
					continue;

				// Check whether triangle is closer than what was previously
				// seen at this pixel
				
				float currz = zABC * pv;
				//currz  /= (DEF * pv);
				if (fb->IsFarther(u, v, currz))
					continue;
				fb->SetZ(u, v, currz);
				
				// pixel is inside triangle and triangle is visible at pixel
				// compute color of pixel based on current triangle
				vector currColor;
				if ( renderMode == SM) 
					continue;
				if ( renderMode == VCI ){
					currColor = vector( rABC * pv, gABC * pv, bABC * pv );
				}else if ( renderMode == MCI ) {
					float r = ModelInterpolation(u, v, rABC, DEF);				
					float g = ModelInterpolation(u, v, gABC, DEF);				
					float b = ModelInterpolation(u, v, bABC, DEF);				
					currColor = vector(r,g,b);
				}
				if ( renderMode == TM) {
					int w = texture->w;
					int h = texture->h;
					float miS = ModelInterpolation(u, v, sABC, DEF);
					float miT = ModelInterpolation(u, v, tABC, DEF);
					miS = (miS * (float)w);
					miT = (miT * (float)h);

					int st = (h-1-(int)(miT)%h) * w+(int)(miS)%w;
					if(st < 0 || st > h*w) continue;
					currColor.SetFromColor(texture->pix[st]);
				}
				//Calculated Color at pixel using phong equation
				for( int li = 0; li < lightsN; li++ ){
					if(!L[li]->on)
						continue;
					vector fullColor;
					fullColor.SetFromColor(color);
					vector currNormal, lv;
					vector pp(pv);
					pp[2] = currz;
					lv = (L[li]->L->C - ppc->UnProject(pp)).norm();
					currNormal = vector(nxABC*pv/(DEF*pv), nyABC*pv/(DEF*pv), nzABC*pv/(DEF*pv)).norm();
					float kd = lv * currNormal;
					kd = (kd < 0.0f) ? 0.0f : kd;
					//currColor = currColor * ka ;
					//currColor = currColor * (ka + (1.0f-ka)*kd);
					
					vector point = ppc->UnProject(pp);
					vector SMp;
					L[li]->L->Project(point, SMp);
					float ul = SMp[0];
					float vl = SMp[1];
					if(L[li]->sm->IsOutsideFrame(ul, vl)) {
						currColor = currColor * ka ;
						//fb->Set(u,v, currColor.GetColor());
						continue;
					}
					//Check depth from light to calculate shadows
					float e = SMp[2]*.01;
					if(L[li]->sm->IsFarther((int)ul, (int)vl, SMp[2]+e)) {
						currColor = currColor * ka ;
						//fb->Set(u,v, currColor.GetColor());
					}else {
						currColor *= ka +(1.0f-ka)*kd;
						//fb->Set(u,v, currColor.GetColor());
					}
				}
				fb->Set(u,v, currColor.GetColor());
			}
		}
	}
	delete []pverts;

}

/*
 *	The Function to render the mesh using 
 *	the Hardware programing pipline.
 */
void TMesh::RenderHW() {
	if (RenderMode == WF)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDisable(GL_LIGHTING);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, (float*)verts);
	if (normals) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, (float*)normals);
	}
	if (cols) {
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, (float*)cols);
	}
	if (tcs && RenderMode == TM) {
		glEnable(GL_TEXTURE_2D);
		glDisableClientState(GL_COLOR_ARRAY);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, scene->texName[0]);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, tcs);
		glFlush();
		GLenum err;
		while((err = glGetError()) != GL_NO_ERROR) {
			cerr << "TM" << err << endl;
		}

	}
	/*
	if (scene->lightsN) {
		glEnable(GL_LIGHTING);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
	}
	*/
	glDrawElements(GL_TRIANGLES, 3*trisN, GL_UNSIGNED_INT, tris);
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void TMesh::SetEEQS(vector pv0, vector pv1, vector pv2, vector *eeqs) {

	vector pvs[3];
	pvs[0] = pv0;
	pvs[1] = pv1;
	pvs[2] = pv2;
	for ( int i = 0; i < 3; i++ ) {
		int i1 = (i+1)%3;
		int i2 = (i+2)%3;
		eeqs[i][0] = -pvs[i][1] + pvs[i1][1]; //-Ay + By;
		eeqs[i][1] =  pvs[i][0] - pvs[i1][0]; // Ay - By;
		eeqs[i][2] = -pvs[i][0]*eeqs[i][0] - pvs[i][1]*eeqs[i][1];

		vector v2(pvs[i2][0], pvs[i2][1], 1.0f);
		if(eeqs[i]*v2 < 0.0f)
			eeqs[i] *= -1.0f;
	}
}


matrix TMesh::ComputeRastMatrix(PPC* ppc, vector v0, vector v1, vector v2) {
	matrix ABC;
	ABC.setCol(0,ppc->a); ABC.setCol(1,ppc->b); ABC.setCol(2,ppc->c);
	matrix Q;
	Q.setCol(0,v0-ppc->C); Q.setCol(1,v1-ppc->C); Q.setCol(2,v2-ppc->C);
	Q = invert(Q);
	Q = Q * ABC;
	return Q;
}
void TMesh::SetModelRast(matrix Q, vector r, vector *ABC) {
	float rastA = Q(0) * r;
	float rastB = Q(1) * r;
	float rastC = Q(2) * r;
	*ABC = vector( rastA, rastB, rastC );

}
float TMesh::ModelInterpolation(float u, float v, vector ABC, vector DEF) {
	float ret;
	//		A			B			C
	ret = ABC[0]*u + ABC[1]*v + ABC[2];
	//		D			E			F
	ret = ret/(DEF[0]*u + DEF[1]*v + DEF[2]);
	return ret;
}

void TMesh::LoadBin(char *fname) {

	ifstream ifs(fname, ios::binary);
	if (ifs.fail()) {
		cerr << "INFO: cannot open file: " << fname << endl;
		return;
	}

	ifs.read((char*)&vertsN, sizeof(int));
	char yn;
	ifs.read(&yn, 1); // always xyz
	if (yn != 'y') {
		cerr << "INTERNAL ERROR: there should always be vertex xyz data" << endl;
		return;
	}
	if (verts)
		delete verts;
	verts = new vector[vertsN];

	ifs.read(&yn, 1); // cols 3 floats
	if (cols)
		delete cols;
	cols = 0;
	if (yn == 'y') {
		cols = new vector[vertsN];
	}
	ifs.read(&yn, 1); // normals 3 floats
	if (normals)
		delete normals;
	normals = 0;
	if (yn == 'y') {
		normals = new vector[vertsN];
	}

	ifs.read(&yn, 1); // texture coordinates 2 floats
	if (tcs)
		delete tcs;
	tcs = 0;
	if (yn == 'y') {
		tcs = new float[vertsN*2];
	}

	ifs.read((char*)verts, vertsN*3*sizeof(float)); // load verts

	if (cols) {
		ifs.read((char*)cols, vertsN*3*sizeof(float)); // load cols
	}

	if (normals)
		ifs.read((char*)normals, vertsN*3*sizeof(float)); // load normals

	if (tcs) 
	{
		ifs.read((char*)tcs, vertsN*2*sizeof(float)); // load texture coordinates
	}

	ifs.read((char*)&trisN, sizeof(int));
	if (tris)
		delete tris;
	tris = new unsigned int[trisN*3];
	ifs.read((char*)tris, trisN*3*sizeof(unsigned int)); // read tiangles

	ifs.close();

	cerr << "INFO: loaded " << vertsN << " verts, " << trisN << " tris from " << endl << "      " << fname << endl;
	cerr << "      xyz " << ((cols) ? "rgb " : "") << ((normals) ? "nxnynz " : "") << ((tcs) ? "tcstct " : "") << endl;

	SetAABB();

	enabled = true;

}

void TMesh::SetAABB() {

	if (aabb) {
		delete aabb;
	}

	aabb = new AABB(verts[0]);

	for (int vi = 0; vi < vertsN; vi++)
		aabb->AddPoint(verts[vi]);

	//cerr << "TMesh AABB: " << aabb->corners[0] << endl << aabb->corners[1] << endl;

}

void TMesh::Translate(vector tv) {

	for (int vi = 0; vi < vertsN; vi++)
		verts[vi] = verts[vi] + tv;
	SetAABB();
}

void TMesh::Rotate(vector axis, float theta) {
	vector center = GetCenter();
	for (int vi = 0; vi < vertsN; vi++){
	//vector d = center - axis;
		verts[vi].rotatePoint(center, axis, theta);
		normals[vi].rotateVector(axis, theta);
	}
	SetAABB();
}

vector TMesh::GetCenter() {

	vector ret(0.0f, 0.0f, 0.0f);
	for( int i = 0; i < vertsN; i++ ) {
		ret = ret + verts[i];
	}
	ret = ret / vertsN;
	return ret;

}

vector TMesh::GetCenterOfBB() {
	vector ret = (aabb->corners[0] + aabb->corners[1])/2.0f;
	return ret;
}

void TMesh::Position(vector newCenter) {

	vector oldCenter = GetCenter();
	Translate(newCenter-oldCenter);
	SetAABB();

}

void TMesh::ScaleToNewDiagonal(float newDiagonal) {

	float oldDiagonal = (aabb->corners[1] - aabb->corners[0]).length();
	float sf = newDiagonal / oldDiagonal;
	float Length();
	vector oldCenter = GetCenter();
	Position(vector(0.0f, 0.0f, 0.0f));
	Scale(sf);
	Position(oldCenter);
	SetAABB();

}

void TMesh::Scale(float scf) {

	vector center = GetCenter();
	for (int vi = 0; vi < vertsN; vi++) {
		vector len = verts[vi] - center;
		len = len * scf;
		verts[vi] = center + len;
	}

	SetAABB();

}
