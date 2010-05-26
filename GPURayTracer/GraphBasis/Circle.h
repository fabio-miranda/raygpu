/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Cabecalho Line.h
   referencia: Implementação Line.H de Cesar Tadeu Pozzer
   Contem a definição da classe Line
\**********************************************************/

#ifndef __CIRCLE___H__
#define __CIRCLE___H__

#define PI_2 6.28318530717958
#define DIV  30

#include "GraphBasis/Vector3.h"

class Circle
{
   Vector3 _pos;//position (p) and radius (r).
   float  _ray;

   Vector3 *_refPos;
public:
   Circle(Vector3 pos, float ray);

   void setRefPos(Vector3 *refPos);
   Vector3* getRefPos();

   bool intersects(Circle &circ, Vector3 &dir);

   void setPos(Vector3 pos);
   Vector3 getPos();

   void setRadius(float ray);
   float getRadius();

   void render();
};

#endif
