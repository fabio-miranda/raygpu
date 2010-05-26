/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once
#include <string>

#include "GraphBasis/Vector3.h"
#include "GraphBasis/Color.h"

using namespace std;

class RTMaterial
{
   static int materialNum;


   protected:
      Color mSpecular;
      Color mDiffuse;
      float mSpecularExp;
      float mReflective;
      float mRefractive;
      float mOpacity;

      int myRTMaterialNum;
   public:
      RTMaterial();
      RTMaterial(Color specular, Color diffuse, float specularExp,
         float reflective, float refractive, float opacity);

      int getMyRTMaterialNumber() const;
      void readFromStr(char buffer[]);

};

