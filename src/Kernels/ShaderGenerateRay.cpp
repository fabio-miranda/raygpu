#include "ShaderGenerateRay.h"

ShaderGenerateRay::ShaderGenerateRay(){

}

ShaderGenerateRay::ShaderGenerateRay(std::string name, std::string pack) : Shader(name, pack){

	m_locEyePos = getUniformLoc("eyePos");
	m_locEyeDir = getUniformLoc("eyeDir");

}

ShaderGenerateRay::~ShaderGenerateRay(){

}

void ShaderGenerateRay::enable(Vector3 eyePos, Vector3 eyeDir){

	Shader::setActive(true);
	glUniform3f(m_locEyePos, eyePos.x, eyePos.y, eyePos.z);
	glUniform3f(m_locEyeDir, eyeDir.x, eyeDir.y, eyeDir.z);
}

void ShaderGenerateRay::disable(){
	Shader::setActive(false);
}