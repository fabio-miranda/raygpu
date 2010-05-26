/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Cabecalho Plane.h
   referencia: Implementação Plane.H Contem a definição
   da classe Plane
\**********************************************************/
#ifndef _ANALITIC_PLANE_H_
#define _ANALITIC_PLANE_H_


#include "GraphBasis/Vector3.h"

#include "GraphObject3D/GraphObject3D.h"

class AnaliticPlane
{
   Vector3 _n,_pos;
   Vector3 *_refPos;
public:
   AnaliticPlane(){}
   AnaliticPlane(Vector3 normal, Vector3 point);
   //calculo de intesercao entre linhas
   //s e t representam as coordenadas parametricas do ponto de intersecao.
//   bool intersects(AnaliticPlane &p, float &s, float &t);
//   bool intersects(Circle &circ, float &s, float &t);

   void setRefPos(Vector3 *refPos);
   Vector3* getRefPos();

   void setPos(Vector3 _p);
   Vector3 getPos();

   double getD();

   void setNormal(Vector3 _n);
   Vector3 getNormal();



};

#endif
