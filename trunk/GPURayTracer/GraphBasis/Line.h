/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Cabecalho Line.h
   referencia: Implementação Line.H de Cesar Tadeu Pozzer
   Contem a definição da classe Line
\**********************************************************/
#ifndef __LINE___H__
#define __LINE___H__



#include "GraphBasis/Vector3.h"
#include "GraphBasis/Circle.h"
#include "GraphBasis/AnaliticPlane.h"

#include "GraphObject3D/GraphObject3D.h"

class Line
{
   Vector3 _pos, _dir; //position (p) and direction (d). Direction is a vector
                //with lenght equals line lenght in respect to the origin.
   Vector3 *_refPos;

   static Vector3 defaultVec;
public:
   Line(Vector3 pos, Vector3 dir);
   //calculo de intesercao entre linhas
   //s e t representam as coordenadas parametricas do ponto de intersecao.
   bool intersects(Line &l, float &s, float &t);
   bool intersects(Circle &circ, float &s, float &t);
   bool intersects(AnaliticPlane &p, Vector3& q );

   void setRefPos(Vector3 *refPos);
   Vector3* getRefPos() const;

   void setPos(Vector3 _p);
   Vector3 getPos() const;

   void setDir(Vector3 _d);
   Vector3 getDir() const;

   void render();
   //plota o ponto de intersecao em funcao de s.
   //b indica se houve ou nao intesecao.
//   void renderIntersection(float s, bool b);
};


std::ostream &operator<<(std::ostream & os, const Line & l);
#endif
