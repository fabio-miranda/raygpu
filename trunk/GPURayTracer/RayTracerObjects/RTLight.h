/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once

#include <string>

#include "GraphBasis/Vector3.h"
#include "GraphBasis/Color.h"

using namespace std;

class RTLight
{
   static int lightNum;


   protected:
      Vector3 mPos;
      Color mSpecular;
      Color mDiffuse;

      int myRTLightNum;
   public:
      enum {Max_Lights = 8};

      RTLight();
      RTLight(Vector3 pos, Color specular, Color diffuse);

      Vector3 getPosition() const;

      Color getSpecularColor() const;

      Color getDiffuseColor() const;

      int getMyRTLightNumber() const;
      void readFromStr(char buffer[]);

};

