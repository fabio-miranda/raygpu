#include "KernelGenerateRay.h"

KernelGenerateRay::KernelGenerateRay(){

}

KernelGenerateRay::KernelGenerateRay(int width, int height, Vector3 bbMin, Vector3 bbMax) : KernelBase("generateRay.vert", "generateRay.frag", width, height){


	//Output
	m_texIdRayPos = addOutput(0);
	m_texIdRayDir = addOutput(1);

	//Input
	m_shader.setActive(true);
		m_locEyePos = addInputVec3("eyePos", Vector3(0, 0, 0));
		m_locEyeDir = addInputVec3("EyeDir", Vector3(0, 0, 0));
		addInputVec3("bbMin", bbMin);
		addInputVec3("bbMax", bbMax);
	m_shader.setActive(false);

}

KernelGenerateRay::~KernelGenerateRay(){

}

void KernelGenerateRay::step(Vector3 eyePos, Vector3 eyeDir){

	m_fbo.setActive(true);
	m_shader.setActive(true);
		glUniform3f(m_locEyePos, eyePos.x, eyePos.y, eyePos.z);
		glUniform3f(m_locEyeDir, eyeDir.x, eyeDir.y, eyeDir.z);
		renderQuad();
	m_shader.setActive(false);
	m_fbo.setActive(false);

}

GLuint KernelGenerateRay::getTexIdRayPos(){return m_texIdRayPos;}
GLuint KernelGenerateRay::getTexIdRayDir(){return m_texIdRayDir;}
