#include "KernelTraverse.h"


KernelTraverse::KernelTraverse(){


}

KernelTraverse::KernelTraverse(int width, int height, Vector3 voxelSize, GLuint texIdGrid, int gridArraySize, Vector3 gridSize, GLuint texIdRayPos, GLuint texIdRayDir)
: KernelBase("./resources/vertice.vert", "./resources/traverse.frag", width, height){

	//Output
	addOutput(0, texIdRayPos);
	addOutput(1, texIdRayDir);
	//addOutput(2, texIdGrid);

	//Input
	m_shader->setActive(true);
		addInputTexture("textureRayPos", texIdRayPos);
		addInputTexture("textureRayDir", texIdRayDir);
		addInputTexture("samplerGridVec", texIdGrid);
		addInputVec3("voxelSize", voxelSize);
		addInputVec3("gridSize", gridSize);
		addInputFloat("gridArraySize", gridArraySize);
	m_shader->setActive(false);


}

KernelTraverse::~KernelTraverse(){

}

