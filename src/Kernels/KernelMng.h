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
#include "RayTracerObjects/RTScene.h"
#include "GraphBasis/Vector3.h"

class KernelMng {

public:
	KernelMng(int width, int height,RTScene* scene, float nearPlaneWidth, float nearPlaneHeight);
	~KernelMng();

	void step(KernelMngState stateToStop, Vector3 eyePos, Vector3 eyeDir, Vector3 eyeUp, Vector3 eyeRight, float nearPlane);
	GLuint getTextureColorId();
	void renderKernelOutput(KernelMngState stateToRender, int outputNum);
	void generateRay();
	KernelMngState m_currentState;
private:
	KernelMngState oracle();
	void update(KernelMngState stateToStop);
	void render(Vector3 eyePos, Vector3 eyeDir, Vector3 eyeUp, Vector3 eyeRight, float nearPlane);
	int countActiveRays();

	KernelGenerateRay* m_kernelGenerateRay;
	KernelTraverse* m_kernelTraverse;
	KernelIntersect* m_kernelIntersect;
	KernelShade* m_kernelShade;

	UniformGrid* m_uniformGrid;
	GLuint m_textureRayPosition;
	GLuint m_textureRayDirection;
	FrameBufferObject m_fbo;

};


#endif