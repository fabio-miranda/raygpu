/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Cabecalho Shader.h
   referencia: Implementação Shader.H de Cesar Tadeu Pozzer
   Contem a definição da classe Shader
\**********************************************************/

#ifndef _SHADER_H_
#define _SHADER_H_



#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>
using namespace std;

class Shader
{
   GLuint p, f, v;
   bool enabled, enablable;
   string _name;
public:
   Shader(string name, string pack);
   Shader(string name, const char *vert, const char *frag);
   ~Shader();

   void setActive(bool);
   bool isActive();

   string getName();

   GLint getUniformLoc(const char *);
   GLint getAttribLoc(const char *);

private:
   void setShaders(const char *vert, const char *frag);
   char *textFileRead(const char *fn);
   int  printOglError(const char *file, int line);
   void printProgramInfoLog(GLuint obj);
   void printShaderInfoLog(GLuint obj);
};


#endif
