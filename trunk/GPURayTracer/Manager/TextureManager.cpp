/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Arquivo TextureManager.cpp
   Contem as funçoes que implementam a classe TextureManager
\**********************************************************/


#include "Manager/TextureManager.h"
#include <iostream>


using namespace std;

TextureManager TexManager;

///Static Vars
vector<string> TextureManager :: imgNames;
//vector<Bmp*>   TextureManager :: dataFiles;
vector<int> TextureManager :: textureCount;
vector<GLuint> TextureManager :: textureIDs;
vector<ImgAttributes> TextureManager :: textureAttributes;


//////////////////
//Public
//////////////////
TextureManager :: TextureManager()
{
}

TextureManager ::  ~TextureManager()
{

}

void TextureManager :: clearAll()
{
   textureCount.clear();
   imgNames.clear();
   textureAttributes.clear();
   for(vector<GLuint>::iterator it(textureIDs.begin());it!=textureIDs.end();++it)
   {
      glDeleteTextures(1,&(*it));
   }
   textureIDs.clear();
}

// funcao velha de carrega textura
/*GLuint  TextureManager :: newTexID(string imgName)
{
   if(imgName.empty())
      return NULL;
   GLuint textureID;
   Bmp *b = new Bmp(imgName);
   if(!b)
      return GL_INVALID_OPERATION;

   glDepthFunc(GL_LEQUAL);
   glGenTextures( 1, &textureID );

   configure(textureID,b);

   imgNames.push_back(imgName);
   dataFiles.push_back(b);
   textureIDs.push_back(textureID);
//   cout << "new "<<imgName<< endl;
   return textureID;
}*/

// funcao nova que carrega jpg
GLuint  TextureManager :: newTexID(string imgName)
{
   GLuint textureID;
   textureID = getAndIncTexID(imgName);
   if(textureID!=GL_INVALID_OPERATION)
   {
      return textureID;
   }

   // daqui pra baixo eh o que precisa pra carrega jpg
   SDL_Surface* surface = IMG_Load(imgName.c_str());
   if(surface == NULL){
      fprintf(stderr, "ERRO: Arquivo de textura não encontrado: %s\n", imgName.c_str());
      return GL_INVALID_OPERATION;
   }

   ImgAttributes imgAttributes;

   glPixelStorei(GL_UNPACK_ALIGNMENT,4);
   glGenTextures(1, &textureID);
   glBindTexture(GL_TEXTURE_2D, textureID);

   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

   glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
   glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   SDL_PixelFormat *format = surface->format;
   if (format->Amask){
      gluBuild2DMipmaps(GL_TEXTURE_2D, 4,
         surface->w, surface->h, GL_RGBA,GL_UNSIGNED_BYTE, surface->pixels);
   }else{
      gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
         surface->w, surface->h, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
   }


   imgAttributes.width = surface->w;
   imgAttributes.height = surface->h;

   SDL_FreeSurface(surface);
   // ateh aqui

   glDepthFunc(GL_LEQUAL);



   textureCount.push_back(1);
   imgNames.push_back(imgName);
   textureIDs.push_back(textureID);
   textureAttributes.push_back(imgAttributes);
//   cout << "TEX primeira "<<imgName<<" Count:1"<<endl;
   return textureID;
}

GLuint TextureManager :: getAndIncTexID(string imgName)
{
   if(imgName.empty())
      return GL_INVALID_OPERATION;
   vector<string>::iterator nameIt = imgNames.begin();
   for(int i=0;nameIt!=imgNames.end();++nameIt,++i)
      if(*nameIt==imgName)
      {
         textureCount.at(i) = textureCount.at(i)+1;
//         cout << "TEX Reaproveitei "<<imgName<<" Counter:"<< textureCount.at(i)<<endl;
//         cout << "reused "<<imgName<< endl;
         return textureIDs.at(i);
      }
   return GL_INVALID_OPERATION;
}


GLuint TextureManager :: getTexID(string imgName)
{
   if(imgName.empty())
      return GL_INVALID_OPERATION;
   vector<string>::iterator nameIt = imgNames.begin();
   for(int i=0;nameIt!=imgNames.end();++nameIt,++i)
      if(*nameIt==imgName)
      {
//         cout << "reused "<<imgName<< endl;
         return textureIDs.at(i);
      }
   return GL_INVALID_OPERATION;
}


void TextureManager :: deleteTexID(string imgName)
{
//   cout << "TEX CallDelete "<< imgName<<endl;
   vector<int>::iterator countIt = textureCount.begin();
   vector<ImgAttributes>::iterator attrIt = textureAttributes.begin();
   vector<GLuint>::iterator idIt = textureIDs.begin();
   vector<string>::iterator nameIt = imgNames.begin();
   for(;nameIt!=imgNames.end();++nameIt,++idIt,++attrIt,countIt++)
      if(*nameIt==imgName)
      {
         *countIt = *countIt-1;

         if(*countIt<=0)
         {
//            cout << "TEX Deletada "<< imgName<<" Count:"<< *countIt<<endl;
            GLuint imgId  = *idIt;
            glDeleteTextures(	1,&imgId);
            textureAttributes.erase(attrIt);
            textureIDs.erase(idIt);
            imgNames.erase(nameIt);
            textureCount.erase(countIt);
         }

//         }else cout << "TEX Decrementada"<<imgName<<" Count:"<<*countIt<<endl;
         return;
      }
}

/*Bmp* TextureManager :: getData(string imgName)
{
   if(imgName.empty())
      return NULL;
   vector<string>::iterator nameIt = imgNames.begin();
   for(int i=0;nameIt!=imgNames.end();++nameIt,++i)
      if(*nameIt==imgName)
      {
//         cout << "reused "<<imgName<< endl;
         return dataFiles.at(i);
      }
   return NULL;
}*/


ImgAttributes TextureManager :: getTexAttributts(string imgName)
{
   if(imgName.empty())
      return ImgAttributes();
   vector<string>::iterator nameIt = imgNames.begin();
   for(int i=0;nameIt!=imgNames.end();++nameIt,++i)
      if(*nameIt==imgName)
      {
//         cout << "reused "<<imgName<< endl;
         return textureAttributes.at(i);
      }
   return ImgAttributes();
}



bool TextureManager :: texExist(string imgName)
{
   if(imgName.empty())
      return false;
   vector<string>::iterator nameIt = imgNames.begin();
   for(;nameIt!=imgNames.end();++nameIt)
      if(*nameIt==imgName)
         return true;
   return false;
}




//////////////
//Private
////////////

/*void TextureManager :: configure(GLuint textureID,Bmp *img)
{
   glBindTexture( GL_TEXTURE_2D, textureID );

   glTexImage2D(GL_TEXTURE_2D,
                0,
                GL_RGB,
                img->getWidth(),
                img->getHeight(),
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                img->getImage() );
}*/
