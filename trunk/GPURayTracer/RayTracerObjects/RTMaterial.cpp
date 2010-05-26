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
{
   myRTMaterialNum = materialNum++;
}

RTMaterial :: RTMaterial(Color specular, Color diffuse, float specularExp,
         float reflective, float refractive, float opacity)
:mSpecular(specular)
,mDiffuse(diffuse)
,mSpecularExp(specularExp)
,mReflective(reflective)
,mRefractive(refractive)
,mOpacity(opacity)
{
   myRTMaterialNum = materialNum++;
}

int RTMaterial :: getMyRTMaterialNumber() const
{
   return myRTMaterialNum;
}

void RTMaterial :: readFromStr(char buffer[])
{
   int r = sscanf( buffer, "%f %f %f %f %f %f %f %f %f %f %*s\n", &mDiffuse.r, &mDiffuse.g, &mDiffuse.b, &mSpecular.r, &mSpecular.g, &mSpecular.b, &mSpecularExp, &mReflective, &mRefractive, &mOpacity);//,* == textureFileName );
   mDiffuse *= 1.0/255;
   mSpecular *= 1.0/255;
   assert(r == 11 - 1);//,-1 == textureFileName );
//   cout << "Material num, pos, Diffuse, Specular, SpecularExp, Reflective, Refractive, Opacity:\n"<< myRTMaterialNum << endl << mDiffuse << mSpecular << mSpecularExp << endl <<mReflective << endl << mRefractive << endl << mOpacity <<endl;
}
