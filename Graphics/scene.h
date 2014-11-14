#pragma once
#pragma comment(lib,"fltk.lib")
#pragma comment(lib,"fltkgl.lib")
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"libtiff.lib")
#pragma comment(lib, "cg.lib")
#pragma comment(lib, "cgGL.lib")

#include "CGInterface.h"
#include "gui.h"
#include "framebuffer.h"
#include "ppc.h"
#include "tmesh.h"
#include "light.h"
#include "LIBTIFF\libtiff.h"
#include <string>

#define BLACK	0xFF000000
#define WHITE	0xFFFFFFFF
#define RED		0xFF0000FF
#define GREEN	0xFF00FF00
#define BLUE	0xFFFF0000

class Scene {
public:
  
  CGInterface *cgi; 
  ReflectionShader *shader;
  GUI *gui;
  FrameBuffer *fb;
  FrameBuffer *hwfb;
  PPC *ppc;
  TMesh **tmeshes;
  int tmeshesN;
  int lightsN;
  Light **lights;
  int texturesN;
  FrameBuffer **textures;
  GLuint *texName;
  float ka;
  bool HWinit;

  Scene();
  void open( const char* );
  void save( const char* );
  void openTexture( const char* imageName, FrameBuffer *texutre);
  void DBG();
  void Render();
  void RenderShadow();

  void RenderHW();
  void InitializeTextures();
  void InitializeLights();
  //void InitializeShadowMap();

  void blur();
  void emboss();
  void sharpen();
  void edge();
  void brightness(float);
  void contrast(float);
  void rasterShape(int, int, int, int, unsigned int);
};

extern Scene *scene;

