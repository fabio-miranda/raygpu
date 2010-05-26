/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Arquivo Line
   Contem a implementaçao dos metodos da classe Line.cpp
\**********************************************************/

#include <iostream>


#include "GraphBasis/Line.h"
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>

Vector3 Line :: defaultVec(0,0,0);

///////////////////
//~ Line
//////////////////
Line :: Line(Vector3 pos, Vector3 dir)
{
   _pos = pos;
   _dir = dir;
   _refPos = &defaultVec;
}


void Line :: setRefPos(Vector3 *refPos)
{
   _refPos = refPos;
}

Vector3* Line :: getRefPos() const
{
   return _refPos;
}

   //calculo de intesercao entre linhas
   //s e t representam as coordenadas parametricas do ponto de intersecao.
bool Line :: intersects(Line &l, float &s, float &t)
{

   Vector3 diff = (*(l.getRefPos())+l.getPos())-(*_refPos+_pos);
   Vector3 lDir = l.getDir();
   Vector3 cross = _dir^lDir;

   double c = ~cross; c*=c;

   if (c > 0.001 * ~_dir * ~(l.getDir()))
   {

      double det1 = diff.x*lDir.y*cross.z + diff.y*lDir.z*cross.x + diff.z*lDir.x*cross.y
                    -diff.z*lDir.y*cross.x - diff.y*lDir.z*cross.x - diff.x*lDir.z*cross.y;

      double det2 = diff.x*_dir.y*cross.z + diff.y*_dir.z*cross.x + diff.z*_dir.x*cross.y
                  -diff.z*_dir.y*cross.x - diff.y*_dir.z*cross.x - diff.x*_dir.z*cross.y;
      s = det1/c;
      t = det2/c;
      if( t > 0 && t < 1 && s > 0 && s < 1 )
         return true;
   }

   return false;

/*
   //determinant - diz se o sistema eh LI ou LD
   float Det = l.getDir().x*_dir.z - l.getDir().z*_dir.x;
   Vector3 lrefPos = *(l.getRefPos());
//   lrefPos.y=0;

   Vector3 mrefPos = (*_refPos);
//   mrefPos.y=0;
   Vector3 P1 = mrefPos +_pos;
   Vector3 P2 = lrefPos + l.getPos();

   Vector3 Diff = lrefPos + l.getPos() - ( mrefPos +_pos);

   //se determinante for zero, daria erro de vivisao por zero.
   //Ocorre quanto direcoes sao coincidentes, ou seja, linhas parelelas.
   //Neste caso s ou t seriam infinito
   if (Det*Det > 0.001 * ~_dir * ~(l.getDir()))
   {
      double InvDet = 1.0/Det;
      s = (l.getDir().x*Diff.z - l.getDir().z*Diff.x)*InvDet;
      t = (_dir.x*Diff.z - _dir.z*Diff.x)*InvDet;

      if( t > 0 && t < 1 && s > 0 && s < 1 )
         return true;
   }
   else //linhas paralelas
   {
      return false;
   }
   return false;*/
}


/**
 * Verifica se uma circunferencia centrada em c.p com diametro definido por c.r é interceptada por
 * uma linha com ponto inicial em p e com o vetor modulo/direcao d. A equação retorna os pontos de
 * intersecao s e t.
 */
bool Line :: intersects(Circle &circ, float &s, float &t)
{
   float a, b, c, r2, delta;
   // é subtraido o centro da circunferencia para centrarmos a circunferencia na origem
   Vector3 centro((_refPos->x + _pos.x) - (circ.getRefPos()->x + circ.getPos().x),  0.0l ,(_refPos->z + _pos.z) - (circ.getRefPos()->z + circ.getPos().z));

   // o raio ao quadrado da circunferencia
   r2 = circ.getRadius() * circ.getRadius();

   // a da quadrica
   a = _dir.x*_dir.x + _dir.z*_dir.z;
   // b da quadrica
   b = 2*(centro.x*_dir.x + centro.z*_dir.z);
   // c da quadrica
   c = centro.x*centro.x + centro.z*centro.z - r2;

   // calcula o delta da quadrica
   delta = b*b - 4*a*c;

   // se negativo nao colide em hipotese alguma
   if( delta < 0 )
      return false;
   else
   {
      // calcula os pontos de interseccao
      s = ((-b)+sqrt(delta)) / (2*a);
      t = ((-b)-sqrt(delta)) / (2*a);

      if( (t > 0 && t < 1) || (s > 0 && s < 1) )
         return true;
   }
   return false;
}

bool Line :: intersects(AnaliticPlane &p, Vector3 &q)
{
   Vector3 pPos = *p.getRefPos()+p.getPos();
   Vector3 pNor = p.getNormal().unitary();
   double d = pNor*pPos;

   double c = pNor*_dir.unitary();

   if(c==0.0) //Parallel
      return false;

   double s = (d-(*_refPos+_pos)*pNor)/c;

   if(s<0)
   {
      return false;
   }
   q = (*_refPos+_pos)+(_dir.unitary())*s;
   return true;
}

void Line :: setPos(Vector3 p)
{
   _pos = p;
}

Vector3 Line :: getPos() const
{
   return _pos;
}


void Line :: setDir(Vector3 d)
{
   _dir = d;
}

Vector3 Line :: getDir() const
{
   return _dir;
}


void Line :: render()
{
   Vector3 pf;
   pf = *_refPos+_pos + _dir;
   float prevWidth;
   glGetFloatv(GL_LINE_WIDTH, &prevWidth);

   glLineWidth(5);
   glBegin(GL_LINES);
      glColor3f(1,0,0);
      glVertex3f(_pos.x, _pos.y, _pos.z);
      glVertex3f(pf.x, pf.y, pf.z);
   glEnd();
   glLineWidth(prevWidth);
}

//plota o ponto de intersecao em funcao de s.
//b indica se houve ou nao intesecao.
//   void Line :: renderIntersection(float s, bool b)
//   {
//      if (b == false )
//         return;
//
//      calcula o ponto de intesecao na linha.
//      Vector p1;
//      p1 = p + d*s;
//
//      glPointSize(10);
//      glBegin(GL_POINTS);
//         glVertex2f(p1.x, p1.y);
//      glEnd();
//   }





std::ostream &operator<<(std::ostream & os, const Line & l)
{
   return os   << "(P\n" << *(l.getRefPos())+l.getPos() <<",D" << l.getDir() << ")"<<std::endl;
}



