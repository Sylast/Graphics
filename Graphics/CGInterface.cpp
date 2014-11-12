#define GEOM_SHADER

#include "CGInterface.h"
#include "vector.h"
#include "scene.h"

#include <iostream>

using namespace std;

CGInterface::CGInterface() : needInit(true) {};

void CGInterface::PerSessionInit() {
	CGprofile latestVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
	CGprofile latestGeometryProfile = cgGLGetLatestProfile(CG_GL_GEOMETRY);
	CGprofile latestPixelProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);

	if (latestGeometryProfile == CG_PROFILE_UNKNOWN) {
		cerr << "ERROR: geometry profile is not available" << endl;
#ifdef GEOM_SHADER
		exit(0);
#endif
	}

	cgGLSetOptimalOptions(latestGeometryProfile);
	CGerror Error = cgGetError();
	if (Error) {
		cerr << "CG ERROR: " << cgGetErrorString(Error) << endl;
	}

	cout << "Infor: Latest GP Profile Supported: " << cgGetProfileString(latestGeometryProfile) << endl;

	geometryCGprofile = latestGeometryProfile;

	cout << "Infor: Latest VP Profile Supported: " << cgGetProfileString(latestVertexProfile) << endl;
	cout << "Infor: Latest FP Profile Supported: " << cgGetProfileString(latestPixelProfile) << endl;

	vertexCGprofile = latestVertexProfile;
	pixelCGprofile = latestPixelProfile;
	cgContext = cgCreateContext();

	needInit = false;
}

void CGInterface::EnableProfiles() {
	cgGLEnableProfile(vertexCGprofile);
	cgGLEnableProfile(pixelCGprofile);
#ifdef GEOM_SHADER
	cgGLEnableProfile(geometryCGprofile);
#endif
}

void CGInterface::DisableProfiles(){
	cgGLDisableProfile(vertexCGprofile);
	cgGLDisableProfile(pixelCGprofile);
#ifdef GEOM_SHADER
	cgGLDisableProfile(geometryCGprofile);
#endif
}

bool FullShader::PerSessionInit(CGInterface *cgi) {
#ifdef GEOM_SHADER
	geometryProgram = cgCreateProgramFromFile(cgi->cgContext, CG_SOURCE, 
		"CG/FullShader.cg", cgi->geometryCGprofile, "GeometryMain", NULL);
	if (geometryProgram == NULL)  {
		CGerror Error = cgGetError();
		cerr << "Shader One Geometry Program COMPILE ERROR: " << cgGetErrorString(Error) << endl;
		cerr << cgGetLastListing(cgi->cgContext) << endl << endl;
		return false;
	}
#endif

	vertexProgram = cgCreateProgramFromFile(cgi->cgContext, CG_SOURCE, 
		"CG/FullShader.cg", cgi->vertexCGprofile, "VertexMain", NULL);
	if (vertexProgram == NULL) {
		CGerror Error = cgGetError();
		cerr << "Shader One Geometry Program COMPILE ERROR: " << cgGetErrorString(Error) << endl;
		cerr << cgGetLastListing(cgi->cgContext) << endl << endl;
		return false;
	}

	fragmentProgram = cgCreateProgramFromFile(cgi->cgContext, CG_SOURCE, 
		"CG/FullShader.cg", cgi->pixelCGprofile, "FragmentMain", NULL);
	if (fragmentProgram == NULL)  {
		CGerror Error = cgGetError();
		cerr << "Shader One Fragment Program COMPILE ERROR: " << cgGetErrorString(Error) << endl;
		cerr << cgGetLastListing(cgi->cgContext) << endl << endl;
		return false;
	}

#ifdef GEOM_SHADER
	cgGLLoadProgram(geometryProgram);
#endif
	cgGLLoadProgram(vertexProgram);
	cgGLLoadProgram(fragmentProgram);

	vertexModelViewProj = cgGetNamedParameter( vertexProgram, "modelViewProj" );
	geometryModelViewProj = cgGetNamedParameter( geometryProgram, "modelViewProj" );

	return true;

}

void FullShader::PerFrameInit() {
	
	cgGLSetStateMatrixParameter( vertexModelViewProj, 
		CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY );

	cgGLSetStateMatrixParameter( geometryModelViewProj, 
		CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY );

}

void FullShader::PerFrameDisable() {}

void FullShader::BindPrograms() {
	cgGLBindProgram( vertexProgram );
	cgGLBindProgram( fragmentProgram );
#ifdef GEOM_SHADER
	cgGLBindProgram( geometryProgram );
#endif
}