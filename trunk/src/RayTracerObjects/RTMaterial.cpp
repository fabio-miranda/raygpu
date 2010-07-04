/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#include <iostream>

#include "RayTracerObjects/RTMaterial.h"

using namespace std;

int RTMaterial :: materialNum = 0;

///////////////////
//~ RTMaterial
//////////////////
RTMaterial :: RTMaterial()
:mCalculed(false)
{
   myRTMaterialNum = materialNum++;
}


int RTMaterial :: getMyRTMaterialNumber() const
{
   return myRTMaterialNum;
}

void RTMaterial :: readFromStr(char buffer[])
{
   int r = sscanf( buffer, "%f %f %f %f %f %f %f %f %f %f %*s\n", &diffuse.r, &diffuse.g, &diffuse.b, &specular.r, &specular.g, &specular.b, &specularExp, &reflective, &refractive, &opacity);//,* == textureFileName );
   diffuse *= 1.0/255;
   specular *= 1.0/255;
   assert(r == 11 - 1);//,-1 == textureFileName );
//   cout << "Material num, pos, Diffuse, Specular, SpecularExp, Reflective, Refractive, Opacity:\n"<< myRTMaterialNum << endl << mDiffuse << mSpecular << mSpecularExp << endl <<mReflective << endl << mRefractive << endl << mOpacity <<endl;
}

void RTMaterial :: configure()
{
   if(!mCalculed)
   {
      mGAmbient[0] = 1.0;
      mGAmbient[1] = 1.0;
      mGAmbient[2] = 1.0;
      mGAmbient[3] = 1.0;

      mGDiffuse[0] = diffuse.r;
      mGDiffuse[1] = diffuse.g;
      mGDiffuse[2] = diffuse.b;
      mGDiffuse[3] = 1.0;

      mGSpecular[0] = specular.r;
      mGSpecular[1] = specular.g;
      mGSpecular[2] = specular.b;
      mGSpecular[3] = 1.0;

      mGShi = specularExp;
      mCalculed = true;
   }
}

void RTMaterial :: render()
{
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  mGAmbient);
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  mGDiffuse);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mGSpecular);
   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mGShi);
}
