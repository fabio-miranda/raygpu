#include "KernelGenerateRay.h"

KernelGenerateRay::KernelGenerateRay(){

}

KernelGenerateRay::KernelGenerateRay(int width, int height, int gridArraySize, Vector3 gridSize, Vector3 gridVoxelSize, Vector3 bbMin, Vector3 bbMax, float nearPlaneWidth, float nearPlaneHeight)
: KernelBase("./resources/vertice.vert", "./resources/generateRay.frag", width, height){


	//Output
	m_texIdColor = addOutput(0);
  m_texIdRayPos = addOutput(1);
	m_texIdRayDir = addOutput(2);
  

	//Input
	m_shader->setActive(true);
		m_locEyePos = addInputVec3("eyePos", Vector3(0, 0, 0));
		m_locEyeDir = addInputVec3("eyeDir", Vector3(0, 0, 0));
		m_locEyeUp = addInputVec3("eyeUp", Vector3(0, 0, 0));
		m_locEyeRight = addInputVec3("eyeRight", Vector3(0, 0, 0));
		m_locNearPlanePos = addInputFloat("nearPlane", 0);
		m_locNearPlaneSize = addInputVec3("nearPlaneSize", Vector3(nearPlaneWidth, nearPlaneHeight, 0.0f));
		m_locScreenSize = addInputVec3("screenSize", Vector3(width, height, 0));
		addInputVec3("bbMin", bbMin);
		addInputVec3("bbMax", bbMax);
		addInputVec3("gridSize", gridSize);
		addInputVec3("gridVoxelSize", gridVoxelSize);
		addInputFloat("gridArraySize", (float)gridArraySize);
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

}

GLuint KernelGenerateRay::getTexIdRayPos(){return m_texIdRayPos;}
GLuint KernelGenerateRay::getTexIdRayDir(){return m_texIdRayDir;}

GLuint KernelGenerateRay::getTexIdColor()
{
  return m_texIdColor;
}


