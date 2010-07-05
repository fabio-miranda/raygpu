#ifndef KernelIntersect_H
#define KernelIntersect_H

#include "Kernels/KernelBase.h"
#include "SpacePartition/UniformGrid.h"
#include "GraphBasis/FrameBufferObject.h"

class KernelIntersect : public KernelBase {

public:
  KernelIntersect();

  KernelIntersect(int width, int height, GLuint texIdRayPos, GLuint texIdRayDir,
                  GLuint texIdShadowInfo, GLuint texIdGrid,
                  GLuint texIdTriangleList, GLuint texIdvertexes, GLfloat gridTexSize, 
                  GLfloat triangleListTexSize, GLfloat vertexesTexSize);
	~KernelIntersect();

   
  GLuint getTexIdTriangleHitInfo() const;
  
private:
  GLuint m_texIdTriangleHitInfo;
};


#endif