/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Arquivo Circle
   Contem a implementaçao dos metodos da classe Circle.cpp
\**********************************************************/

#include <iostream>


#include "GraphBasis/Line.h"
#include <GL/glut.h>
#include <math.h>


Circle :: Circle(Vector3 pos, float ray)
{
   _pos = pos;
   _ray = ray;
   _refPos = new Vector3(0,0,0);
}


void Circle :: setRefPos(Vector3 *refPos)
{
   _refPos = refPos;
}

Vector3* Circle :: getRefPos()
{
   return _refPos;
}


bool Circle :: intersects(Circle &circ,Vector3 &dir)
{
   Vector3 diff = (*(circ.getRefPos()) + circ.getPos()) - (*_refPos+ _pos);

   if((~diff)<=(_ray+circ.getRadius()))
   {
      dir = diff.unitary();
      return true;
   }
   dir = Vector3(0,0,0);
   return false;
}

void Circle :: setPos(Vector3 pos)
{
   _pos = pos;
}

Vector3 Circle :: getPos()
{
   return _pos;
}

void Circle :: setRadius(float ray)
{
   _ray = ray;
}

float  Circle :: getRadius()
{
   return _ray;
}

void Circle :: render()
{
   float x1, y1;
   float inc = PI_2/DIV;
   glBegin(GL_LINE_LOOP);
      for(float ang=0; ang<PI_2; ang+=inc)
      {
         x1 = (int)(cos(ang)*_ray);
         y1 = (int)(sin(ang)*_ray);
         glVertex2d(x1+_pos.x, y1+_pos.y);
      }
   glEnd();
}
