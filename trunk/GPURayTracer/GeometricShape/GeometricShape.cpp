/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
/**********************************************************\
                  Arquivo GeometricShape
   Contem a implementaçao dos metodos da classe GeometricShape.cpp
\**********************************************************/

#include <iostream>

#include "GraphBasis\Shader.h"
#include "GeometricShape/GeometricShape.h"

using namespace std;

///////////////////
//~ GeometricShape
//////////////////
GeometricShape :: GeometricShape(Shader* s)
:mMinResolutionStep(.01f)
,mShader(s)
{
   mResolution = .5f;
   mResStep = mMinResolutionStep/mResolution;
   mModified = true;
   mObjColor = Color(0,0,0);
   mShaderActive = false;
}

GeometricShape :: ~GeometricShape()
{
}

void GeometricShape :: configure()
{
   if(mModified)
   {
      calcGeometry();
      mModified = false;
   }
}

void GeometricShape :: setShader(Shader *s)
{
   mShader = s;
   calcShaderInit();
}

void GeometricShape :: setShaderActive(bool op)
{
   mShaderActive = op;
}

void GeometricShape :: setColor(Color c)
{
   mObjColor = c;
}

Color GeometricShape :: getColor() const
{
   return  mObjColor;
}

void GeometricShape :: setResolution(double percent)
{
   if(percent>1.0) percent = 1.0;
   if(percent<0.0) percent = mMinResolutionStep;
   mResolution = percent;
   mResStep = mMinResolutionStep/mResolution;
   mModified = true;
}

double GeometricShape :: getResolution() const
{
   return mResolution;
}

