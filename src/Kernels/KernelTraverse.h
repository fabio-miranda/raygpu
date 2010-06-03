#ifndef KernelTraverse_H
#define KernelTraverse_H


#include "Kernels/KernelBase.h"
#include "SpacePartition/UniformGrid.h"
#include "GraphBasis/Shader.h"
#include "GraphBasis/Vector3.h"

class KernelTraverse : public KernelBase {

public:
	KernelTraverse();
	KernelTraverse(int width, int height, Vector3 voxelSize, GLuint texIdGrid, GLuint texIdRayPos, GLuint texIdRayDir);
	~KernelTraverse();

	

private:
	GLuint m_texIdVoxelsIntersection;

};


#endif