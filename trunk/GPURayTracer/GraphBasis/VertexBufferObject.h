/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#pragma once

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>

typedef struct vbobuffer
{
   void* data;
   GLenum type;
   int n;
   int offset;
   GLenum clientState;

   vbobuffer()
      :data(NULL)
      ,type(sizeof(GL_BYTE))
      ,n(0)
      ,offset(0)
      ,clientState(GL_VERTEX_ARRAY)
   {}

   int sizeInBytes()
   {
      return n*sizeof(type);
   }

   void setPointer()
   {
      switch(clientState)
      {
         case GL_NORMAL_ARRAY:
            glNormalPointer(type, 0, (GLvoid*)offset);
         break;
         case GL_COLOR_ARRAY:
         case GL_SECONDARY_COLOR_ARRAY:
            glColorPointer(3, type,  0, (GLvoid*)offset);
         break;
         case GL_VERTEX_ARRAY:
            glVertexPointer(3, type,  0, (GLvoid*)offset);
         break;
         case GL_TEXTURE_COORD_ARRAY:
            glTexCoordPointer(2, type,  0, (GLvoid*)offset);
         break;
         case GL_INDEX_ARRAY:
            glIndexPointer(type, 0, (GLvoid*)offset);
         break;
         case GL_FOG_COORD_ARRAY:
            glFogCoordPointer(type, 0, (GLvoid*)offset);
         break;
         case GL_EDGE_FLAG_ARRAY:
            glEdgeFlagPointer(0, (GLvoid*)offset);
         break;
      }
   }
}VBOBuffer;


using namespace std;

class VertexBufferObject
{
   bool mSupported;
   bool mActive;
   GLuint mvboId;
   GLuint mvboIndicesId;

   GLenum mPrimitive;

   int mVBOBuffersTotalSize;

   int  mModified;
   bool mCalculed;

   vector<VBOBuffer> mVBOBuffers;
   VBOBuffer mVBOIndexBuffer;

public:
   VertexBufferObject(GLenum primitive = GL_TRIANGLES);
   ~VertexBufferObject();

   void clear();

   void configure();
   void render();

   void setVBOBuffer(GLenum clientState, GLenum type, int n, void* data);
   void setVBOIndexBuffer(GLenum type, int n, void* data);
private:

};
