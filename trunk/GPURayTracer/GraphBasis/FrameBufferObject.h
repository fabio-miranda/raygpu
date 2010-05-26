/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#pragma once


#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>

#include "GraphBasis\RenderBufferObject.h"
using namespace std;

typedef struct bufferType{
   enum BindType {Texture, RenderBufferObject, NotBind};
   BindType bindType;
   GLuint id;
   GLenum textureTarget;

   bufferType()
      :bindType(NotBind)
      ,textureTarget(GL_COLOR_ATTACHMENT0_EXT)
      {}

   void bind()
   {
      switch(bindType)
      {
         case Texture:
            glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, textureTarget, GL_TEXTURE_2D, id, 0);
         break;
         case RenderBufferObject:
            glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, textureTarget, GL_RENDERBUFFER_EXT, id);
         break;

         case NotBind:
         default:
         break;
      }
   }
}BufferType;

class FrameBufferObject
{
   bool mSupported;
   bool mActive;
   GLuint mfboId;

   int mWidth;
   int mHeight;

   BufferType mCollorBuffers [GL_MAX_COLOR_ATTACHMENTS_EXT];
   BufferType mDephtBuffer;
   BufferType mStencilBuffer;

   vector<RenderBufferObject> mRenderBuffers;
public:
   FrameBufferObject(int width, int height);
   ~FrameBufferObject();

   GLuint attachToColorBuffer(BufferType::BindType bt, int index = -1, GLuint id = -1);
   GLuint attachToDepthBuffer(BufferType::BindType bt, GLuint id = -1);
   GLuint attachToStencilBuffer(BufferType::BindType bt, GLuint id = -1);

   void setActive(bool);
   bool isActive();

private:
   GLuint createTextureToRender(int width, int height);
   GLuint createRenderFrameObjectToRender(int width, int height);
   bool checkFramebufferStatus();
};


