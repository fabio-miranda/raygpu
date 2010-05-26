/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Cabecalho ShaderVars.h
   Contem a definição das variaveis usadas nos Shaders
\**********************************************************/

#ifndef _SHADER_VARS_H_
#define _SHADER_VARS_H_

#include <GL/gl.h>

#define MAX_GL_LIGHTS 9
//variaveis uniform
extern GLint loc_u_enabledLights[MAX_GL_LIGHTS];
extern GLint loc_u_enabledTexture;
extern GLint loc_u_enabledBump;

//extern GLint loc_a_tangent;
//
extern GLint loc_u_tex;
extern GLint loc_u_bumpTex;


void initShaderVars();


#endif
