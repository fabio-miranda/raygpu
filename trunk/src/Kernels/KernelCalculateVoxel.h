#ifndef KernelCalculate_H
#define KernelCalculate_H


#include "Kernels/KernelBase.h"
#include "SpacePartition/UniformGrid.h"
#include "GraphBasis/Shader.h"
#include "GraphBasis/Vector3.h"

class KernelCalculateVoxel : public KernelBase {

public:
	KernelCalculateVoxel();
	KernelCalculateVoxel(int width, int height, Vector3 voxelSize, Vector3 bbMin, Vector3 bbMax, GLuint texIdGrid, int gridArraySize, Vector3 gridSize, GLuint texIdRayPos, GLuint texIdRayDir);
	~KernelCalculateVoxel();
	GLuint getTexIdIntersectionMax();

	

private:
	GLuint m_texIdIntersectionMax;
	GLuint m_aux;

};


#endif