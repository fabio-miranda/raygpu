/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#pragma once

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>
using namespace std;

class VertexBufferObject
{
   bool mSupported;
   bool mActive;
   GLuint mvboId;
   GLuint mvboIndicesId;

   bool mCalculed;
   bool mIndexEnabled;

public:
   VertexBufferObject();
   ~VertexBufferObject();

   void configure();
   void render();
private:

};
