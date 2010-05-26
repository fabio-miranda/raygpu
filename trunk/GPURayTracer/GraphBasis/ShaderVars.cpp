/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Arquivo ShaderVars
   Contem a declaraçao das variaveis usadas nos Shaders
\**********************************************************/
#include "GraphBasis/ShaderVars.h"
#include "GraphBasis/Shader.h"
#include "Manager/ShaderManager.h"

//variaveis uniform
GLint loc_u_enabledLights[MAX_GL_LIGHTS];
GLint loc_u_enabledTexture;
GLint loc_u_enabledBump;
//
//GLint loc_a_tangent;
//
GLint loc_u_tex;
GLint loc_u_bumpTex;


//   #include <iostream>

void initShaderVars()
{
   Shader * s = ShaderManager::getShader("LightShader");


/**/
   s = ShaderManager::getShader("BumpShader");
//
   loc_u_enabledBump  = s->getUniformLoc("enabledBump");

   loc_u_bumpTex = s->getUniformLoc("bumpTex");

/**/

//   s = ShaderManager::getShader("TexBumpShader");
//
//   loc_u_enabledBump  = s->getUniformLoc("enabledBump");
//
//   loc_u_bumpTex = s->getUniformLoc("bumpTex");
}
