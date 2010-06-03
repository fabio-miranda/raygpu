#ifndef KernelMng_H
#define KernelMng_H

#include "Kernels/KernelBase.h"
#include "Kernels/KernelGenerateRay.h"
#include "Kernels/KernelTraverse.h"
#include "Kernels/KernelIntersect.h"
#include "Kernels/KernelShade.h"
#include "Kernels/KernelMngStates.h"
#include "SpacePartition/UniformGrid.h"
#include "GraphBasis/FrameBufferObject.h"

class KernelMng {

public:
	KernelMng(int width, int height, UniformGrid* uniformGrid);
	~KernelMng();

	void step(Vector3 eyePos, Vector3 eyeDir);

private:
	KernelMngState oracle();
	void update();
	void render(Vector3 eyePos, Vector3 eyeDir);
	int countActiveRays();

	KernelGenerateRay m_kernelGenerateRay;
	KernelTraverse m_kernelTraverse;
	KernelIntersect m_kernelIntersect;
	KernelShade m_kernelShade;
	KernelMngState m_currentState;
	UniformGrid* m_uniformGrid;
	GLuint m_textureRayPosition;
	GLuint m_textureRayDirection;
	FrameBufferObject m_fbo;

};


#endif