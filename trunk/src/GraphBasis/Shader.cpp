/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Arquivo Shader
   Contem a implementaçao dos metodos da classe Shader
\**********************************************************/
#include <GL/glew.h>
#include <iostream>
#include "GraphBasis/Shader.h"
#include "Manager/ShaderManager.h"

using namespace std;

///////////////////
//~ Shader
//////////////////
Shader::Shader(){}

Shader::Shader(string name, string pack)
{
   string vertStr = pack+".vert";
   string fragStr = pack+".frag";
   char *vert = (char*)(vertStr.c_str());
   char *frag = (char*)(fragStr.c_str());

   _name = name;
   enablable = true;

   GLenum err = glewInit();
   if (GLEW_OK != err)
   {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
      enablable = false;
   }
//   fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

   if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
   {
//      printf("Ready for GLSL\n");
   }else
   {
      printf("Not totally ready :( \n");
      enablable = false;
      //exit(1);
   }

   if (glewIsSupported("GL_VERSION_2_0"))
   {
//      printf("Ready for OpenGL 2.0\n");
   }else
   {
      printf("OpenGL 2.0 not supported\n");
      enablable = false;
      //exit(1);
   }

   ShaderManager::insertShader(this);

   setShaders(vert, frag);

   printShaderInfoLog(v);
   printShaderInfoLog(f);
   printProgramInfoLog(p);

   //http://www.delorie.com/gnu/docs/gcc/cpp_21.html
   //http://www.thescripts.com/forum/thread212429.html
   //http://www.codeguru.com/forum/showthread.php?t=231043
   //printf("\n%s  %d  %s  %s \n", __FILE__, __LINE__, __FUNCTION__, __TIMESTAMP__);

   printOglError(__FILE__, __LINE__);

   enabled = false;
}

Shader::Shader(string name, const char *vert, const char *frag)
{
   _name = name;
   enablable = true;
   GLenum err = glewInit();
   if (GLEW_OK != err)
   {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
      enablable = false;
   }
   fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

   if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
      printf("Ready for GLSL\n");
   else
   {
      printf("Not totally ready :( \n");
      enablable = false;
      //exit(1);
   }
   if (glewIsSupported("GL_VERSION_2_0"))
      printf("Ready for OpenGL 2.0\n");
   else
   {
      printf("OpenGL 2.0 not supported\n");
      enablable = false;
      //exit(1);
   }

   ShaderManager::insertShader(this);

   setShaders(vert, frag);

   printShaderInfoLog(v);
   printShaderInfoLog(f);
   printProgramInfoLog(p);

   //http://www.thescripts.com/forum/thread212429.html
   //http://www.delorie.com/gnu/docs/gcc/cpp_21.html
   //http://www.codeguru.com/forum/showthread.php?t=231043
   //printf("\n%s  %d  %s  %s \n", __FILE__, __LINE__, __FUNCTION__, __TIMESTAMP__);
   printOglError(__FILE__, __LINE__);

   enabled = false;
}


Shader :: ~Shader()
{
//   cout << "DESTRUCTOR "<<"Shader" << " " << _name<< endl;
//   cout << "END DESTRUCTOR "<<"Shader" << " " << _name<< endl;
}

//somente UM programa pode estar ativo a cada momento.
void Shader::setActive(bool active)
{
   if (!enablable)
      return;
   if(active)
	  glUseProgram(p);
   else
	  glUseProgram(0); //desbilita o programa "corrente". Nao necessariamente eh o do proprio objeto.
   enabled = active;
}

bool Shader :: isActive()
{
   return enabled;
}

string Shader :: getName()
{
   return _name;
}




GLint Shader::getUniformLoc(const char *nome)
{
   return glGetUniformLocation(p, nome);
}

GLint Shader::getAttribLoc(const char *nome)
{
   return glGetAttribLocation(p, nome);
}


///////////////////
//~ Private
//////////////////

void Shader::setShaders(const char *vert, const char *frag)
{
   char *vs,*fs;

   v = glCreateShader(GL_VERTEX_SHADER);
   f = glCreateShader(GL_FRAGMENT_SHADER);

   //printf("%d %d ", v, f);

   vs = textFileRead(vert);
   fs = textFileRead(frag);

   const char * vv = vs;
   const char * ff = fs;

   glShaderSource(v, 1, &vv, NULL);
   glShaderSource(f, 1, &ff, NULL);

   free(vs);
   free(fs);

   glCompileShader(v);
   glCompileShader(f);

   p = glCreateProgram();

   glAttachShader(p,v); //Um programa pode 0 ou mais Vertex Shaders e 0 ou mais fragment shader
   glAttachShader(p,f); //caso nao tiver nenhum Vertex Shader, por exemplo, utiliza-se o pipeline fixo do OpenGL para esta funcionalidade

   glLinkProgram(p);
   //glUseProgram(p); //o programador deve ativar o programa desejado.
//   printf("\nCriado Programa com id: %d\n", p);
}








char *Shader::textFileRead(const char *fn)
{
   FILE *fp;
   char *content = NULL;

   int count=0;

   if (fn != NULL)
   {
      fp = fopen(fn,"rt");

      if (fp != NULL)
      {
         fseek(fp, 0, SEEK_END);
         count = ftell(fp);
         rewind(fp);

         if (count > 0) {
            content = (char *)malloc(sizeof(char) * (count+1));
            count = (int)fread(content,sizeof(char),count,fp);
            content[count] = '\0';
         }
         fclose(fp);
      }
      else
      {
         printf("\nFile %s not found", fn);
         getchar();
         exit(1);
      }
   }
   //printf("Arquivo %s %d = \n%s\n\n", fn, strlen(content), content);
   return content;
}

int Shader::printOglError(const char *file, int line)
{
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}

void Shader::printShaderInfoLog(GLuint obj)
{
   int infologLength = 0;
   int charsWritten  = 0;
   char *infoLog;

   glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

   if (infologLength > 0)
   {
      infoLog = (char *)malloc(infologLength);
      glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
      printf("%s\n",infoLog);
      free(infoLog);
   }
}

void Shader::printProgramInfoLog(GLuint obj)
{
   int infologLength = 0;
   int charsWritten  = 0;
   char *infoLog;

   glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

   if (infologLength > 0)
   {
      infoLog = (char *)malloc(infologLength);
      glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
      printf("%s\n",infoLog);
      free(infoLog);
   }
}
