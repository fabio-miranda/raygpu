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
	KernelGenerateRay(int width, int height, int gridArraySize, Vector3 gridSize, Vector3 gridVoxelSize, Vector3 bbMin, Vector3 bbMax, float nearPlaneWidth, float nearPlaneHeight);
	~KernelGenerateRay();

	void step(Vector3 eyePos, Vector3 eyeDir, Vector3 eyeUp, Vector3 eyeRight, float nearPlane);
	GLuint getTexIdRayPos();
	GLuint getTexIdRayDir();
  GLuint getTexIdColor();
  

	GLuint m_locEyePos;
	GLuint m_locEyeDir;
	GLuint m_locEyeUp;
	GLuint m_locEyeRight;
	GLuint m_locNearPlaneSize;
	GLuint m_locNearPlanePos;
	GLuint m_locScreenSize;
	GLuint m_texIdRayPos;
	GLuint m_texIdRayDir;
  GLuint m_texIdColor;



};


#endif