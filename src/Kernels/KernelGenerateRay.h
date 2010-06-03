#ifndef KernelGenerateRay_H
#define KernelGenerateRay_H

#include "Kernels/KernelBase.h"
#include "GraphBasis/Vector3.h"
#include "GraphBasis/FrameBufferObject.h"
#include "SpacePartition/UniformGrid.h"
#include "GraphBasis/FrameBufferObject.h"
#include "GraphBasis/Shader.h"

class KernelGenerateRay : public KernelBase {

public:
	KernelGenerateRay();
	KernelGenerateRay(int width, int height, Vector3 bbMin, Vector3 bbMax);
	~KernelGenerateRay();

	void step(Vector3 eyePos, Vector3 eyeDir);
	GLuint getTexIdRayPos();
	GLuint getTexIdRayDir();

	FrameBufferObject m_fbo;
	Shader m_shader;
	GLuint m_locEyePos;
	GLuint m_locEyeDir;
	GLuint m_texIdRayPos;
	GLuint m_texIdRayDir;



};


#endif