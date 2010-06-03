#include "KernelTraverse.h"


KernelTraverse::KernelTraverse(){


}

KernelTraverse::KernelTraverse(int width, int height, Vector3 voxelSize, GLuint texIdGrid, GLuint texIdRayPos, GLuint texIdRayDir)
: KernelBase("traverse.frag", "traverse.vert", width, height){

	//Output
	addOutput(0, texIdRayPos);
	addOutput(1, texIdRayDir);
	m_texIdVoxelsIntersection = addOutput(2);

	//Input
	m_shader.setActive(true);
		addInputTexture("textureRayPos", texIdRayPos);
		addInputTexture("textureRayDir", texIdRayDir);
		addInputVec3("voxelSize", voxelSize);
	m_shader.setActive(false);

}

KernelTraverse::~KernelTraverse(){

}

