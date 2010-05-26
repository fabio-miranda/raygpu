/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
/**********************************************************\
                  Arquivo Sphere
   Contem a implementaçao dos metodos da classe Sphere.cpp
\**********************************************************/

#include <iostream>
#include <cmath>
#include <GL/glew.h>

#include "GeometricShape/Sphere.h"
#include "GraphBasis\Vector3.h"
#include "GraphBasis\Shader.h"

using namespace std;

/// ////////////////
///~ Sphere
/// ///////////////
Sphere :: Sphere(float radius , Shader * s )
:GeometricShape(s)
,mRadius(radius)
{
   mDisList = glGenLists(1);
   mAng = 0;

   if(!mDisList || mDisList == GL_INVALID_VALUE || mDisList == GL_INVALID_OPERATION )
      cout << "Error: Sphere nao pode ser criado pois o openGl nao havia sido iniciado ou algo do tipo"<<endl;

   calcShaderInit();
}

Sphere :: ~Sphere()
{
   if(mDisList!= GL_INVALID_VALUE && mDisList!= GL_INVALID_OPERATION)
      glDeleteLists(mDisList,1);
}

void Sphere::render()
{
   mAng +=.0001;
   if(mShader && mShaderActive)
   {
      mShader->setActive(true);
         glUniform1i(mTextureLoc,0);
         glUniform1f(mAngUniformLoc,mAng);
         glUniform1f(mRadiusUniformLoc,mRadius);
         glCallList(mDisList);
      mShader->setActive(false);
   }else glCallList(mDisList);
}
/// ////////////////
///~ Protected
/// ///////////////
void Sphere :: calcGeometry()
{
   glNewList(mDisList,GL_COMPILE);
      mObjColor.setColor();
      glBegin(GL_QUADS);
         for(float s = 0.0f; s<1.0f; s+=mMinResolutionStep)
            for(float t = 0.0f; t<1.0f; t+=mMinResolutionStep)
            {
               float tt = t;
               float ss = s;
               Vector3 v(ss,tt,0.0f);
               glVertex3f(v.x, v.y, v.z);

               tt = tt+mMinResolutionStep <1.0 ? tt+mMinResolutionStep : 0.0;
               v.y = tt;
               glVertex3f(v.x, v.y, v.z);

               ss = ss+mMinResolutionStep <1.0 ? ss+mMinResolutionStep : 0.0;
               v.x = ss;
               glVertex3f(v.x, v.y, v.z);

               tt = t;
               v.y = tt;
               glVertex3f(v.x, v.y, v.z);
            }
      glEnd();
   glEndList();
}

void Sphere :: calcShaderInit()
{
   if( mShader )
   {
      mTextureLoc = mShader->getUniformLoc("text");
      mAngUniformLoc  = mShader->getUniformLoc("ang");
      mRadiusUniformLoc = mShader->getUniformLoc("radius");
   }
}
