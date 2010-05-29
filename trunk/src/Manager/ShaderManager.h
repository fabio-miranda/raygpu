/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Cabecalho ShaderManager.h
   Contem a definição do controlador de Objetos
   Ele controla janela, keyboard, mouse ...
\**********************************************************/
#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

#include <vector>
#include <string>

#include <GL/glut.h>
#include "GraphBasis/Shader.h"
using namespace std;

class Shader;

class ShaderManager
{
      static vector<string> shaderNames;
      static vector<Shader*> shaderPtrs;
   public:
      ShaderManager();
      ~ShaderManager();

      static void clearAll();

      static void insertShader(Shader *s);
      static void removeShader(Shader* ptr);

      static Shader* getShader(string shaderName);
};


#endif

