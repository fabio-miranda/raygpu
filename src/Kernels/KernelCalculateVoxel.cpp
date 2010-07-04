#include "KernelCalculateVoxel.h"


KernelCalculateVoxel::KernelCalculateVoxel(){


}

KernelCalculateVoxel::KernelCalculateVoxel(int width, int height, Vector3 voxelSize, Vector3 bbMin, Vector3 bbMax, GLuint texIdGrid, int gridArraySize, Vector3 gridSize, GLuint texIdRayPos, GLuint texIdRayDir)
: KernelBase("./resources/vertice.vert", "./resources/calculateVoxel.frag", width, height){

	//Output
	addOutput(0, texIdRayPos);
	addOutput(1, texIdRayDir);
	m_texIdIntersectionMax =  addOutput(2);
	m_aux = addOutput(3);

	//Input
	m_shader->setActive(true);
		addInputTexture(GL_TEXTURE_2D, "samplerRayPos", texIdRayPos);
		addInputTexture(GL_TEXTURE_2D, "samplerRayDir", texIdRayDir);
		addInputTexture(GL_TEXTURE_2D, "samplerGrid", texIdGrid);

		addInputVec3("bbMin", bbMin);
		addInputVec3("bbMax", bbMax);
		addInputVec3("gridVoxelSize", voxelSize);
		addInputVec3("gridSize", gridSize);
		addInputFloat("gridArraySize", (float)gridArraySize);
	m_shader->setActive(false);

}

KernelCalculateVoxel::~KernelCalculateVoxel(){

}

GLuint KernelCalculateVoxel::getTexIdIntersectionMax()
{
  return m_texIdIntersectionMax;
}

