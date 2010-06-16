#include "KernelIntersect.h"

KernelIntersect::KernelIntersect()
{

}

KernelIntersect::KernelIntersect(int width, int height, GLuint texIdRayPos, GLuint texIdRayDir, GLuint texIdGrid,
                                 GLuint texIdTriangleList, GLuint texIdvertexes, GLfloat gridTexSize, 
                                 GLfloat triangleListTexSize, GLfloat vertexesTexSize)
: KernelBase("./resources/vertice.vert", "./resources/intersector.frag", width, height){


	//Output
	addOutput(0, texIdRayDir);
	m_texIdTriangleHitInfo = addOutput(1);

	//Input
	m_shader->setActive(true);
    addInputTexture(GL_TEXTURE_2D, "rayPos", texIdRayPos);
    addInputTexture(GL_TEXTURE_2D, "rayDir", texIdRayDir);
    addInputTexture(GL_TEXTURE_1D, "grid", texIdGrid);
    addInputTexture(GL_TEXTURE_1D, "triangleList", texIdTriangleList);
    addInputTexture(GL_TEXTURE_1D, "vertexes", texIdvertexes);

    addInputFloat("gridSize", gridTexSize);
    addInputFloat("triangleListSize", triangleListTexSize);
    addInputFloat("vertexesSize", vertexesTexSize);
	m_shader->setActive(false);
}

KernelIntersect::~KernelIntersect(){

}

GLuint KernelIntersect::getTexIdTriangleHitInfo() const 
{
	return m_texIdTriangleHitInfo; 
}