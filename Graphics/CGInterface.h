#pragma once

#include <Cg/cgGL.h>
#include <Cg/cg.h>

class CGInterface {

public:
	CGprofile vertexCGprofile;
	CGprofile geometryCGprofile;
	CGprofile pixelCGprofile;
	CGcontext cgContext;

	bool needInit;
	void PerSessionInit();
	CGInterface();
	void EnableProfiles();
	void DisableProfiles();
};


/*
 *This is used a test shader
 *loads geometry, vertex, and fragment
 */
class FullShader {
protected:
	CGprogram geometryProgram;
	CGprogram vertexProgram;
	CGprogram fragmentProgram;

	CGparameter vertexModelViewProj;
	CGparameter geometryModelViewProj;

public:
	FullShader() {};
	bool PerSessionInit(CGInterface *cgi);
	void BindPrograms();
	void PerFrameInit();
	void PerFrameDisable();
};

class ReflectionShader : public FullShader {
	CGparameter fragmentEye;
	CGparameter fragmentv0;
	CGparameter fragmentv1;
	CGparameter fragmentv2;
	CGparameter fragmentv3;
	CGparameter fragmentTexture;
	CGparameter fragmentTextureTile;

public:
	ReflectionShader() {};
	bool PerSessionInit(CGInterface *cgi);
	void PerFrameInit();
};

