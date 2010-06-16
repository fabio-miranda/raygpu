#include "KernelShade.h"

KernelShade::KernelShade()
{

}

KernelShade::KernelShade(int width, int height, GLuint texIdRayDir, GLuint texIdTriangleHitInfo, GLuint texIdvertexes, GLuint texIdNormals, 
                          GLuint texIdDiffuseTex, GLuint texIdSpecularTex, GLuint texIdLights, 
                          GLfloat normalsTexSize, GLfloat vertexesTexSize, GLfloat diffuseTexSize, GLfloat specularTexSize,
                          GLfloat lightsTexSize)
: KernelBase("./resources/vertice.vert", "./resources/shade.frag", width, height){
      //Output
      addOutput(0, texIdRayDir);    
      m_texIdColor = addOutput(1);

      //Input
      m_shader->setActive(true);
        addInputTexture(GL_TEXTURE_1D, "triangleInfo", texIdTriangleHitInfo);
        addInputTexture(GL_TEXTURE_1D, "vertexes", texIdvertexes);
        addInputTexture(GL_TEXTURE_1D, "normals", texIdNormals);
        addInputTexture(GL_TEXTURE_1D, "diffuseTex", texIdDiffuseTex);
        addInputTexture(GL_TEXTURE_1D, "especularTex", texIdSpecularTex);
        addInputTexture(GL_TEXTURE_1D, "lights", texIdLights);

        addInputFloat("normalsSize", normalsTexSize);
        addInputFloat("vertexesSize", vertexesTexSize);
        addInputFloat("diffuseSize", diffuseTexSize);
        addInputFloat("especularSize", specularTexSize);
        addInputFloat("lightsSize", lightsTexSize);
        m_locEyePos = addInputVec3("eyePos", Vector3(0, 0, 0));
      m_shader->setActive(false);
}

KernelShade::~KernelShade(){

}

void KernelShade::step(Vector3 eyePos){

  m_fbo->setActive(true);
  m_shader->setActive(true);
    glUniform3f(m_locEyePos, eyePos.x, eyePos.y, eyePos.z);
    renderQuad();
  m_shader->setActive(false);
  m_fbo->setActive(false);
}

GLuint KernelShade::getTextureColorId(){
	return m_texIdColor;
}