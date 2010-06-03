#ifndef ShaderGenerateRay_H
#define ShaderGenerateRay_H

#include <iostream>

#include "Kernels/KernelBase.h"
#include "GraphBasis/Vector3.h"
#include "GraphBasis/FrameBufferObject.h"
#include "SpacePartition/UniformGrid.h"


class ShaderGenerateRay : public Shader {

public:
	ShaderGenerateRay();
	ShaderGenerateRay(std::string name, std::string pack);
	~ShaderGenerateRay();
	void enable(Vector3 eyePos, Vector3 eyeDir);
	void disable();

private:
	GLuint m_locEyePos;
	GLuint m_locEyeDir;


};


#endif