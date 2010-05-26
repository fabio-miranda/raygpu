/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
/**********************************************************\
                  Cabecalho Sphere.h
   Contem a definição da classe Sphere
\**********************************************************/
#pragma once

#include <string>
#include <GL/gl.h>
#include "GeometricShape\GeometricShape.h"
#include "GraphBasis/Color.h"

using namespace std;

class Sphere: public GeometricShape
{
   protected:
      GLuint mDisList;
      float mRadius;
      float mAng;

      GLint mTextureLoc;
      GLint mAngUniformLoc ;
      GLint mRadiusUniformLoc;

   public:
      Sphere(float radius = 10.0,Shader * s = NULL);
      virtual ~Sphere();

      virtual void render();
   protected:
      virtual void calcGeometry();
      virtual void calcShaderInit();
};













