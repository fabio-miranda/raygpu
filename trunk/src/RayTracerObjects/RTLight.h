/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once

#include <string>
#include "assert.h"

#include "GraphBasis/Vector3.h"
#include "GraphBasis/Color.h"


#include "Light/PointLight.h"



using namespace std;

class RTLight
{
   static int lightNum;


   protected:
      PointLight mPLight;

      bool mCalculed;

      Vector3 mPos;
      Color mSpecular;
      Color mDiffuse;

      int myRTLightNum;
   public:
      enum {Max_Lights = 8};

      RTLight();

      int getMyRTLightNumber() const;
      void readFromStr(char buffer[]);

      void configure();
      void render();

};

