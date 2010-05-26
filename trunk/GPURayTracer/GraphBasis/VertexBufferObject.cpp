/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#include <GL/glew.h>
#include <iostream>
#include "GraphBasis/VertexBufferObject.h"

using namespace std;

///////////////////
//~ VertexBufferObject
//////////////////
VertexBufferObject::VertexBufferObject()
:mActive(false)
,mCalculed(false)
,mIndexEnabled(false)
{
   mSupported = true;
   GLenum err = glewInit();
   if (GLEW_OK != err)
   {
      mSupported = false;
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
   }

   if (!glewIsSupported("GL_VERSION_2_0"))
   {
      mSupported = false;
      printf("OpenGL 2.0 not supported\n");
   }

   if(glGenBuffersARB && glBindBufferARB && glBufferDataARB && glBufferSubDataARB &&
           glMapBufferARB && glUnmapBufferARB && glDeleteBuffersARB && glGetBufferParameterivARB)
   {
      mSupported = false;
      cout << "Video card does NOT support GL_ARB_vertex_buffer_object." << endl;
   }

   assert(mSupported);
}

VertexBufferObject :: ~VertexBufferObject()
{
   if(mSupported)
   {
      glDeleteBuffersARB(1, &mvboId);
      if(mIndexEnabled)
         glDeleteBuffersARB(1, &mvboIndicesId);
   }
}

void VertexBufferObject :: configure()
{
   if(!mCalculed)
   {
      glGenBuffersARB(1, &mvboId);

      glBindBufferARB(GL_ARRAY_BUFFER_ARB, mvboId);
      // glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vertices)+sizeof(normals)+sizeof(colors), 0, GL_STATIC_DRAW_ARB);
      // glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, sizeof(vertices), vertices);                             // copy vertices starting from 0 offest
      // glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vertices), sizeof(normals), normals);                // copy normals after vertices
      // glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vertices)+sizeof(normals), sizeof(colors), colors);  // copy colours after normals


      if(mIndexEnabled)
      {
         glGenBuffersARB(1, &mvboIndicesId);
         glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mvboIndicesId);
         // glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_indicesSize*sizeof(GLushort), indices, GL_STATIC_DRAW_ARB);
      }

      mCalculed = true;
   }
}

void VertexBufferObject :: render()
{
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, mvboId);
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mvboIndicesId);

  // glEnableClientState(GL_NORMAL_ARRAY);
  // glEnableClientState(GL_COLOR_ARRAY);
  // glEnableClientState(GL_SECONDARY_COLOR_ARRAY);
  // glEnableClientState(GL_VERTEX_ARRAY);
  // glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  // glEnableClientState(GL_INDEX_ARRAY);
  // glEnableClientState(GL_FOG_COORD_ARRAY);
  // glEnableClientState(GL_EDGE_FLAG_ARRAY);

  // glNormalPointer(GL_FLOAT, 0, (void*)sizeof(vertices));
  // glColorPointer(3, GL_FLOAT, 0, (void*)(sizeof(vertices)+sizeof(normals)));
  // glColorPointer(3, GL_FLOAT, 0, (void*)(sizeof(vertices)+sizeof(normals)));
  // glVertexPointer(3, GL_FLOAT, 0, 0);
  // glTexCoordPointer()
  // glIndexPointer()
  // glFogCoordPointer()
  // glEdgeFlagPointer()


  // glDrawArrays(GL_QUADS, 0, 24);
  // glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, indices);


  // glDisableClientState(GL_NORMAL_ARRAY);
  // glDisableClientState(GL_COLOR_ARRAY);
  // glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
  // glDisableClientState(GL_VERTEX_ARRAY);
  // glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  // glDisableClientState(GL_INDEX_ARRAY);
  // glDisableClientState(GL_FOG_COORD_ARRAY);
  // glDisableClientState(GL_EDGE_FLAG_ARRAY);

  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}





















