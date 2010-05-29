/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#pragma once

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#include "assert.h"
#include <string>
using namespace std;

class RenderBufferObject
{
   bool mSupported;
   GLuint mrboId;

   int mWidth;
   int mHeight;
public:
   RenderBufferObject(int width, int height);
   ~RenderBufferObject();

   GLuint getId();
private:
};


