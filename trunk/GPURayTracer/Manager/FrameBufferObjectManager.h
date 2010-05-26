/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#ifndef _FRAMEBUFFEROBJECT_MANAGER_H_
#define _FRAMEBUFFEROBJECT_MANAGER_H_

#include <vector>
#include <string>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <SDL/SDL_image.h>

using namespace std;

class FrameBufferObjectManager
{
      static vector<GLuint> mFBOIDs;
      static vector<int> mFBOCount;
   public:
      FrameBufferObjectManager();
      ~FrameBufferObjectManager();

      static void clearAll();

      static GLuint newFBOID(string imgName);

      static GLuint getFBOID(string imgName);

      static void deleteFBOID(string imgName);

   private:
      static GLuint getAndIncFBOID(string imgName);
};


#endif
