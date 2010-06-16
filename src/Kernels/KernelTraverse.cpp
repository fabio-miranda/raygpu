#include "KernelTraverse.h"


KernelTraverse::KernelTraverse(){


}

KernelTraverse::KernelTraverse(int width, int height, Vector3 voxelSize, GLuint texIdGrid, int gridArraySize, Vector3 gridSize, GLuint texIdRayPos, GLuint texIdRayDir, GLuint texIdGridIntersectionMax, GLuint texIdGridIntersectionMin)
: KernelBase("./resources/vertice.vert", "./resources/traverse.frag", width, height){

	//Output
	addOutput(0, texIdRayPos);
	addOutput(1, texIdRayDir);
	GLuint aux = addOutput(2);

	//Input
	m_shader->setActive(true);
		addInputTexture(GL_TEXTURE_2D, "samplerRayPos", texIdRayPos);
		addInputTexture(GL_TEXTURE_2D, "samplerRayDir", texIdRayDir);
		addInputTexture(GL_TEXTURE_2D, "samplerGridIntersectionMax", texIdGridIntersectionMax);
    addInputTexture(GL_TEXTURE_2D, "samplerGridIntersectionMin", texIdGridIntersectionMin);
    addInputTexture(GL_TEXTURE_1D, "samplerGrid", texIdGrid);
		addInputVec3("voxelSize", voxelSize);
		addInputVec3("gridSize", gridSize);
		addInputFloat("gridArraySize", gridArraySize);
	m_shader->setActive(false);

}

KernelTraverse::~KernelTraverse(){

}

