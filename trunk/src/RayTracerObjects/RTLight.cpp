/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#include <iostream>

#include "RayTracerObjects/RTLight.h"

using namespace std;

int RTLight :: lightNum = 0;
///////////////////
//~ RTLight
//////////////////
RTLight :: RTLight()
:mCalculed(false)
{
   myRTLightNum = lightNum++;

   if(myRTLightNum >= Max_Lights)
      myRTLightNum = myRTLightNum%Max_Lights;
}



int RTLight :: getMyRTLightNumber() const
{
   return myRTLightNum;
}


void RTLight :: readFromStr(char buffer[])
{
   int r = sscanf( buffer, "%f %f %f %f %f %f\n", &mPos.x, &mPos.y, &mPos.z,
      &mDiffuse.r, &mDiffuse.g, &mDiffuse.b);
   mDiffuse *= 1.0/255;
   mSpecular = mDiffuse;
   assert(r == 6);
//   cout << "Light num, pos, Diffuse, Specular:\n"<<myRTLightNum << endl << mPos  << mDiffuse << mSpecular <<endl;
}

void RTLight :: configure()
{
   if(!mCalculed)
   {
      mPLight.setAmbientColor(mDiffuse*.2);
      mPLight.setDiffuseColor(mDiffuse);
      mPLight.setSpecularColor(mSpecular);
      mPLight.setPosition(mPos);
      mCalculed = true;
   }
   mPLight.configure();
}

void RTLight :: render()
{
   mPLight.render();
}

