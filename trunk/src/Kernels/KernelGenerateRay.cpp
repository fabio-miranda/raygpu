#include "KernelGenerateRay.h"

KernelGenerateRay::KernelGenerateRay(){

}

KernelGenerateRay::KernelGenerateRay(int width, int height, Vector3 bbMin, Vector3 bbMax)
: KernelBase("./resources/vertice.vert", "./resources/generateRay.frag", width, height){


	//Output
	m_texIdRayPos = addOutput(0);
	m_texIdRayDir = addOutput(1);
	m_texIdIntersectionMin = addOutput(2);
	m_texIdIntersectionMax = addOutput(3);

	//Input
	m_shader->setActive(true);
		m_locEyePos = addInputVec3("eyePos", Vector3(0, 0, 0));
		m_locEyeDir = addInputVec3("eyeDir", Vector3(0, 0, 0));
		m_locEyeUp = addInputVec3("eyeUp", Vector3(0, 0, 0));
		m_locEyeRight = addInputVec3("eyeRight", Vector3(0, 0, 0));
		m_locNearPlanePos = addInputFloat("nearPlane", 0);
		m_locScreenSize = addInputVec3("screenSize", Vector3(width, height, 0));
		addInputVec3("bbMin", bbMin);
		addInputVec3("bbMax", bbMax);
	m_shader->setActive(false);

}

KernelGenerateRay::~KernelGenerateRay(){

}

void KernelGenerateRay::step(Vector3 eyePos, Vector3 eyeDir, Vector3 eyeUp, Vector3 eyeRight, float nearPlane){

	
  m_fbo->setActive(true);
	m_shader->setActive(true);
		glUniform3f(m_locEyePos, eyePos.x, eyePos.y, eyePos.z);
		glUniform3f(m_locEyeDir, eyeDir.x, eyeDir.y, eyeDir.z);
		glUniform3f(m_locEyeUp, eyeUp.x, eyeUp.y, eyeUp.z);
		glUniform3f(m_locEyeRight, eyeRight.x, eyeRight.y, eyeRight.z);
		glUniform1f(m_locNearPlanePos, nearPlane);
    renderQuad();

	m_shader->setActive(false);
	m_fbo->setActive(false);


	float value0[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  	m_fbo->setActive(true);
    glReadBuffer(m_fbo->m_drawBuffers[0]);
    glReadPixels(0, 0, 1, 1, GL_RGBA, GL_FLOAT, value0);
	m_fbo->setActive(false);
  std::cout <<  "Buffer 0: " << value0[0] << ", " << value0[1] << ", " << value0[2] << ", " << value0[3] << "\n";


	float value1[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	m_fbo->setActive(true);
	  glReadBuffer(m_fbo->m_drawBuffers[1]);
	  glReadPixels(0, 0, 1, 1, GL_RGBA, GL_FLOAT, value1);
	  std::cout <<  "Buffer 1: " << value1[0] << ", " << value1[1] << ", " << value1[2] << ", " << value1[3] << "\n";
	m_fbo->setActive(false);

  
  
	float value2[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	m_fbo->setActive(true);
	  glReadBuffer(m_fbo->m_drawBuffers[2]);
	  glReadPixels(0, 0, 1, 1, GL_RGBA, GL_FLOAT, value2);
	  std::cout << "Buffer 2: " << value2[0] << ", " << value2[1] << ", " << value2[2] << ", " << value2[3] << "\n";
	m_fbo->setActive(false);

	float value3[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	m_fbo->setActive(true);
	  glReadBuffer(m_fbo->m_drawBuffers[3]);
	  glReadPixels(0, 0, 1, 1, GL_RGBA, GL_FLOAT, value3);
	  std::cout <<  "Buffer 3: " << value3[0] << ", " << value3[1] << ", " << value3[2] << ", " << value3[3] << "\n";
	m_fbo->setActive(false);

  cout << endl;

}

GLuint KernelGenerateRay::getTexIdRayPos(){return m_texIdRayPos;}
GLuint KernelGenerateRay::getTexIdRayDir(){return m_texIdRayDir;}
