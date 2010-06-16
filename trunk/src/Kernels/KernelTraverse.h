#ifndef KernelTraverse_H
#define KernelTraverse_H


#include "Kernels/KernelBase.h"
#include "SpacePartition/UniformGrid.h"
#include "GraphBasis/Shader.h"
#include "GraphBasis/Vector3.h"

class KernelTraverse : public KernelBase {

public:
	KernelTraverse();
	KernelTraverse(int width, int height, Vector3 voxelSize,GLuint texIdGrid, int gridArraySize, Vector3 gridSize, GLuint texIdRayPos, GLuint texIdRayDir, GLuint texIdGridIntersectionMax, GLuint texIdGridIntersectionMin);
	~KernelTraverse();

	

private:
	GLuint m_texIdVoxelsIntersection;

};


#endif