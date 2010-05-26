/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Arquivo AnaliticPlane
   Contem a implementaçao dos metodos da classe AnaliticPlane.cpp
\**********************************************************/

#include <iostream>


#include "GraphBasis/AnaliticPlane.h"
#include <GL/glut.h>
#include <math.h>

///////////////////
//~ AnaliticPlane
//////////////////
AnaliticPlane :: AnaliticPlane(Vector3 normal, Vector3 pos)
{
   _pos = pos;
   _n = normal;

   _refPos = new Vector3(0,0,0);
}


void AnaliticPlane :: setRefPos(Vector3 *refPos)
{
   _refPos = refPos;
}

Vector3* AnaliticPlane :: getRefPos()
{
   return _refPos;
}

void AnaliticPlane :: setPos(Vector3 p)
{
   _pos = p;
}

Vector3 AnaliticPlane :: getPos()
{
   return _pos;
}

double AnaliticPlane :: getD()
{
   return _n*((*_refPos)+_pos);
}

void AnaliticPlane :: setNormal(Vector3 n)
{
   _n = n;
}

Vector3 AnaliticPlane :: getNormal()
{
   return _n;
}




