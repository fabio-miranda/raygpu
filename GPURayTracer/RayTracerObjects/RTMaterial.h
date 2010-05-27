/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once
#include <string>
#include <GL/gl.h>

#include "GraphBasis/Vector3.h"
#include "GraphBasis/Color.h"

using namespace std;

class RTMaterial
{
   static int materialNum;


   protected:
      bool mCalculed;

      GLfloat mGAmbient[4];
      GLfloat mGSpecular[4];
      GLfloat mGDiffuse[4];
      GLfloat mGShi;

      Color mSpecular;
      Color mDiffuse;
      float mSpecularExp;
      float mReflective;
      float mRefractive;
      float mOpacity;

      int myRTMaterialNum;
   public:
      RTMaterial();

      int getMyRTMaterialNumber() const;
      void readFromStr(char buffer[]);

      void configure();
      void render();

};

