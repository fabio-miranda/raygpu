#include "Kernels/KernelBase.h"

KernelBase::KernelBase(){

}

KernelBase::KernelBase(char* frag, char* vert, int width, int height){
	m_shader = Shader("", frag, vert);
	m_fbo = FrameBufferObject(width, height);
}

KernelBase::~KernelBase(){

}

GLuint KernelBase::addInputTexture(char* name, GLuint id){

	GLuint loc = m_shader.getUniformLoc(name);
	glUniform1iARB(loc, m_inputTextures.size());
	m_inputTextures.push_back(id);

	return loc;
}

GLuint KernelBase::addInputVec3(char* name, Vector3 value){
	
	GLuint loc = m_shader.getUniformLoc(name);
	glUniform3fARB(loc, value.x, value.y, value.z);

	return loc;
}

GLuint KernelBase::addInputFloat( char* name, GLfloat value )
{
  GLuint loc = m_shader.getUniformLoc(name);
  glUniform1f(loc, value);

  return loc;
}

GLuint KernelBase::addOutput(int index, GLuint textureId){

	return m_fbo.attachToColorBuffer(bufferType::Texture, index, textureId);

}

void KernelBase::activateTextures(){

	for(int i=0; i<m_inputTextures.size(); i++){
		glActiveTextureARB(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D,m_inputTextures.at(i));
	}

}

void KernelBase::renderQuad(){
  glBegin(GL_QUADS);
    glVertex3f(0,0,0); glTexCoord2f(0,0);
    glVertex3f(1,0,0); glTexCoord2f(1,0);
    glVertex3f(1,1,0); glTexCoord2f(1,1);
    glVertex3f(0,1,0); glTexCoord2f(0,1);
  glEnd();
}

void KernelBase::step(){

	m_fbo.setActive(true);
	m_shader.setActive(true);
	activateTextures();
	renderQuad();
	m_shader.setActive(false);
	m_fbo.setActive(false);

}


