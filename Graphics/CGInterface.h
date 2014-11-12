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

class ReflectionShader {
	CGprogram vertexProgram;

public:
	ReflectionShader() {};
	bool PerSessionInit(CGInterface *cgi);
	void BindPrograms();
	void PerFrameInit();
	void PerFrameDisable();
};

