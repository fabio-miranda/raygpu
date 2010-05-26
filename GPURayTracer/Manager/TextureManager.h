/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Cabecalho TextureManager.h
   Contem a definição do controlador de Objetos
   Ele controla janela, keyboard, mouse ...
\**********************************************************/
#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#include <vector>
#include <string>

#include <GL/gl.h>
#include <GL/glut.h>
#include <SDL/SDL_image.h>
#include <unistd.h>

using namespace std;

typedef struct
{
   int width;
   int height;
}ImgAttributes;

class TextureManager
{
      static vector<string> imgNames;
      //static vector<Bmp*> dataFiles; // sem putaria de bmp
      static vector<GLuint> textureIDs;
      static vector<int> textureCount;
      static vector<ImgAttributes> textureAttributes;
   public:
      TextureManager();
      ~TextureManager();

      static void clearAll();

      static GLuint newTexID(string imgName);

      static GLuint getTexID(string imgName);

      static ImgAttributes getTexAttributts(string imgName);

      static void deleteTexID(string imgName);

      //Bmp* getData(string imgName); // saca fora

      static bool texExist(string imgName);
   private:
      //void configure(GLuint textureID,Bmp *img);
      static GLuint getAndIncTexID(string imgName);
};


#endif
