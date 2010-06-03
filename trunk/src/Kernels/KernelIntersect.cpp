#include "KernelIntersect.h"

KernelIntersect::KernelIntersect()
{

}

KernelIntersect::KernelIntersect(int width, int height, GLuint texIdRayPos, GLuint texIdRayDir, GLuint texIdGrid,
                                 GLuint texIdTriangleList, GLuint texIdvertexes, GLfloat gridTexSize, 
                                 GLfloat triangleListTexSize, GLfloat vertexesTexSize)
: KernelBase("IntersectorKernel.frag", "IntersectorKernel.vert", width, height){
  //Output
  m_texIdTriangleHitInfo = addOutput(0);

  //Input
  m_shader.setActive(true);
    addInputTexture("rayPos", texIdRayPos);
    addInputTexture("rayDir", texIdRayDir);
    addInputTexture("grid", texIdGrid);
    addInputTexture("triangleList", texIdTriangleList);
    addInputTexture("vertexes", texIdvertexes);

    addInputFloat("gridSize", gridTexSize);
    addInputFloat("triangleListSize", triangleListTexSize);
    addInputFloat("vertexesSize", vertexesTexSize);
  m_shader.setActive(false);
}

KernelIntersect::~KernelIntersect(){

}

GLuint KernelIntersect::getTexIdTriangleHitInfo() const 
{
  return m_texIdTriangleHitInfo; 
}