/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Arquivo ShaderManager.cpp
   Contem as funçoes que implementam a classe ShaderManager
\**********************************************************/

#include <iostream>
#include "Manager/ShaderManager.h"

#include "GraphBasis/Shader.h"

using namespace std;

ShaderManager ShdManager;

//Instancia de variaveis necessarias em funçoes estaticas
vector<string> ShaderManager :: shaderNames;
vector<Shader*> ShaderManager :: shaderPtrs;
//**********************

//////////////////
//Public
//////////////////
ShaderManager :: ShaderManager()
{
}

ShaderManager ::  ~ShaderManager()
{

}


void ShaderManager ::  clearAll()
{
   shaderPtrs.clear();
   shaderNames.clear();
}



Shader* ShaderManager :: getShader(string name)
{
   vector<string>::iterator nameIt = shaderNames.begin();
   for(int i=0;nameIt!=shaderNames.end();++nameIt,++i)
      if(*nameIt==name)
         return shaderPtrs.at(i);
   cout << "warnig: "<< name <<": SHADER NOT FOUND"<<endl;
   return NULL;
}





void ShaderManager :: insertShader(Shader* ptr)
{
   if(!ptr)
      return;
   string name = ptr->getName();
   vector<string>::iterator nameIt = shaderNames.begin();
   for(int i=0;nameIt!=shaderNames.end();++nameIt,++i)
      if(*nameIt==name)
      {
         cout << "warnig: SHADER NAME ALREADY EXIST "<< name <<endl;
         return;
      }
//   cout << name <<endl;
   shaderPtrs.push_back(ptr);
   shaderNames.push_back(name);
}

void ShaderManager :: removeShader(Shader* ptr)
{
   if(!ptr)
      return;
   vector<Shader*>::iterator ptrIt = shaderPtrs.begin();
   vector<string>::iterator nameIt = shaderNames.begin();
   for(;ptrIt!=shaderPtrs.end();++ptrIt,++nameIt)
      if(*ptrIt==ptr)
      {
         shaderPtrs.erase(ptrIt);
         shaderNames.erase(nameIt);
         return;
      }
   cout << "SOMETHING IS WRONG! DELETED SHADER WASN'T IN THE OBJMANAGER LIST\n";

}


