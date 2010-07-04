#include "Kernels/KernelBase.h"

KernelBase::KernelBase(){

}

KernelBase::KernelBase(char* vert, char* frag, int width, int height){
	m_shader = new Shader(vert, frag);
	m_fbo = new FrameBufferObject(width, height);
}

KernelBase::~KernelBase(){

	delete m_shader;
	delete m_fbo;

}

GLuint KernelBase::addInputTexture(GLenum textureDimension, char* name, GLuint id){

  GLuint loc = m_shader->getUniformLocation(name);
  glUniform1iARB(loc, m_inputTextures.size());
  m_inputTextures.push_back(std::pair<GLenum, GLuint>(textureDimension, id));

  return loc;
}

GLuint KernelBase::addInputVec3(char* name, Vector3 value){
	
	GLuint loc = m_shader->getUniformLocation(name);
	glUniform3fARB(loc, value.x, value.y, value.z);

	return loc;
}

GLuint KernelBase::addInputVec3(char* name, Color value){

  GLuint loc = m_shader->getUniformLocation(name);
  glUniform3fARB(loc, value.r, value.g, value.b);

  return loc;
}

GLuint KernelBase::addInputFloat( char* name, GLfloat value )
{
  GLuint loc = m_shader->getUniformLocation(name);
  glUniform1f(loc, value);

  return loc;
}

GLuint KernelBase::addOutput(int index, GLuint textureId){
	GLuint id = m_fbo->attachToColorBuffer(bufferType::Texture, index, textureId);
	m_outputTextures.push_back(id);
	return id;

}

GLuint KernelBase::getOutputTexture(int index){
	if(index < m_outputTextures.size())
		return m_outputTextures.at(index);
	else
		return m_outputTextures.at(m_outputTextures.size()-1);
}

void KernelBase::activateTextures(){
	
  GLenum e = glGetError();
  for(int i=0; i<m_inputTextures.size(); i++){
		glActiveTextureARB(GL_TEXTURE0 + i);
		glBindTexture(m_inputTextures.at(i).first, m_inputTextures.at(i).second);
	}

  e = glGetError();
}

void KernelBase::renderQuad(){
  glBegin(GL_QUADS);
    glTexCoord2f(0,0);glVertex3f(0,0,0); 
    glTexCoord2f(1,0);glVertex3f(1,0,0); 
    glTexCoord2f(1,1);glVertex3f(1,1,0); 
    glTexCoord2f(0,1);glVertex3f(0,1,0); 
  glEnd();
}

void KernelBase::step(){

	m_fbo->setActive(true);
	m_shader->setActive(true);
	activateTextures();
	renderQuad();
	m_shader->setActive(false);
	m_fbo->setActive(false);

}


