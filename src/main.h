#ifndef Main_H
#define Main_H

#include <iostream>
#include "GL\glew.h"
#include <GL\freeglut.h>

#include "GraphBasis\Vector3.h"
#include "RayTracerObjects\RTScene.h"
#include "Kernels\KernelMng.h"

void render();
void init(int argc, char *argv[]);
void renderAxis();
int main(int argc, char *argv[]);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void keyboardSpecial(int key, int x, int y);
void mouseButtons(int button, int state, int x, int y);
void mouseActive(int x, int y);

#endif