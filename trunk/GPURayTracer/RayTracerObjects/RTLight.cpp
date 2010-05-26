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
{
   myRTLightNum = lightNum++;

   if(myRTLightNum >= Max_Lights)
      myRTLightNum = myRTLightNum%Max_Lights;
}

RTLight :: RTLight(Vector3 pos, Color specular, Color diffuse)
:mPos(pos)
,mSpecular(specular)
,mDiffuse(diffuse)
{
   myRTLightNum = lightNum++;

   if(myRTLightNum >= Max_Lights)
      myRTLightNum = myRTLightNum%Max_Lights;
}


Vector3 RTLight :: getPosition() const
{
   return mPos;
}


Color RTLight :: getSpecularColor() const
{
   return mSpecular;
}


Color RTLight :: getDiffuseColor() const
{
   return mDiffuse;
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


