#pragma once

#include "vector.h"
#include "framebuffer.h"
#include "ppc.h"
#include "light.h"

//RenderModes
#define WF		0
#define VCI		1 //vector color interpolation
#define MCI		2
#define PPDL	3 //per pixel diffuse lighting
#define TM		4 //texture mapping
#define SM		5

class TMesh {
public:
  bool enabled;
  int RenderMode;
  vector *verts, *normals, *cols;
  int vertsN;
  unsigned int *tris;
  int trisN;
  float *tcs;
  AABB *aabb;
  int texIndex;
  bool shade;
  vector dims;

  TMesh() : verts(0), vertsN(0), tris(0), trisN(0), normals(0), 
			cols(0), tcs(0), aabb(0), enabled(false), RenderMode(0),
			texIndex(-1), shade(false), dims(){};
  TMesh(vector center, vector dims, unsigned int color);
  TMesh(vector center, vector dims, unsigned int color, int texIndex, float tile);

  void LoadBin(char *fname);
  void RenderPoints(PPC *ppc, FrameBuffer *fb, int psize);
  void RenderWireframe(PPC *ppc, FrameBuffer *fb, unsigned int color);
  void RenderFilled(PPC *ppc, FrameBuffer *fb, unsigned int color, int lightsN, Light **L, float ka, FrameBuffer *texture, int renderMode);
  void RenderModel(PPC *ppc, FrameBuffer *fb, int renderMode);
  void RenderHW();

  void SetEEQS(vector pv0, vector pv1, vector pv2, vector *eeqs);
  matrix ComputeRastMatrix(PPC *ppc, vector v0, vector v1, vector v2);
  void SetModelRast(matrix Q, vector r, vector *ABC);
  float ModelInterpolation(float u, float v, vector ABC, vector DEF);
  
  void SetAABB();
  
  void Translate(vector tv);
  void Rotate(vector axis, float theta);

  vector GetCenter();
  vector GetCenterOfBB();
  void Position(vector newCenter);
  void ScaleToNewDiagonal(float newDiagonal);
  void Scale(float scf);
};
