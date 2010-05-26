/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
/**********************************************************\
                  Cabecalho Torus.h
   Contem a definição da classe Torus
\**********************************************************/
#pragma once

#include <string>
#include <GL/gl.h>
#include "GeometricShape\GeometricShape.h"
#include "GraphBasis/Color.h"

using namespace std;

class Torus: public GeometricShape
{
   protected:
      GLuint mDisList;
      float mRadiusIn, mRadiusOut;
      float mAng;

      GLint mAngUniformLoc;
      GLint mTextureLoc;
      GLint mRadiusInUniformLoc;
      GLint mRadiusOutUniformLoc;

   public:
      Torus(float radiusIn = 3.0, float radiusOut = 10.0,Shader * s = NULL);
      virtual ~Torus();

      virtual void render();
   protected:
      virtual void calcGeometry();
      virtual void calcShaderInit();
};












