/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
/**********************************************************\
                  Cabecalho GeometricShape.h
   Contem a definição da classe GeometricShape
\**********************************************************/
#pragma once

#include <string>
#include "GraphBasis/Color.h"

using namespace std;
class Shader;

class GeometricShape
{
   protected:
      const float mMinResolutionStep;
      float mResolution;
      float mResStep;
      bool mModified;

      Shader *mShader;
      bool mShaderActive;

      Color mObjColor;
   public:
      GeometricShape(Shader* s = NULL);
      virtual ~GeometricShape();

      virtual void render() = 0;
      virtual void configure();

      void setShaderActive(bool op);

      void setShader(Shader *s);

      void setColor(Color c);
      Color getColor() const;

      void setResolution(double percent);
      double getResolution() const;
   protected:
      virtual void calcGeometry() = 0;
      virtual void calcShaderInit() = 0;
};











