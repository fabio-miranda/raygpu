#ifndef Main_H
#define Main_H

#include <iostream>
#include "GL\glew.h"
#include <GL\freeglut.h>
#include <iup.h>
#include <iupgl.h>

#include "GraphBasis\Vector3.h"
#include "RayTracerObjects\RTScene.h"
#include "Kernels\KernelMng.h"
#include "Config.h"

int render(Ihandle* handle);
void init(int argc, char *argv[]);
void initIup(int argc, char *argv[]);
void initGL(int argc, char *argv[]);
void renderAxis();
void renderGL(Vector3 eye, Vector3 up);
int main(int argc, char *argv[]);
int reshape(Ihandle* handle, int w, int h);
void keyboard(unsigned char key, int x, int y);
void keyboardSpecial(int key, int x, int y);
void mouseButtons(Ihandle* handle, int button, int state, int x, int y);
void mouseActive(Ihandle* handle, int x, int y);
void createScenes(std::string sceneFileName);
void openFile(Ihandle* handle);
int setGenerateRay(Ihandle* handle, int state);
int setCalculateVoxel(Ihandle* handle, int state);
int setTraverse(Ihandle* handle, int state);
int setIntersect(Ihandle* handle, int state);
int setShade(Ihandle* handle, int state);
int singleStepCallback(Ihandle* handle);
int setContinuousStep(Ihandle* handle, int state);

int lastMousePosX;
int lastMousePosY;
int mouseState;
int mouseButton;

float camAlpha;
float camBeta;
float camR;
float camInc;

float nearPlane;
float fov;
bool singleStep;
bool continuousStep;
bool stateToggles[5];

int rObj;

string sceneFileName;

RTScene* rtScene;
KernelMng* kernelMng;
Ihandle* iupDialog;
Ihandle* canvasRenderer;

#endif