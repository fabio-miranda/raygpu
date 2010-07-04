#ifndef KernelShade_H
#define KernelShade_H

#include "Kernels/KernelBase.h"

class KernelShade : public KernelBase {

public:
	KernelShade();
  KernelShade(int width, int height,  GLuint texIdRayDir, GLuint texIdRayPos,GLuint texIdTriangleHitInfo,
    GLuint texIdvertexes, GLuint texIdNormals, GLuint texIdMaterialTex, GLuint texIdLights, 
    GLfloat normalsTexSize, GLfloat vertexesTexSize, GLfloat materialTexSize,
    GLfloat lightsTexSize, Color clearColor
   );
	~KernelShade();

	void step(Vector3 eyePos);
	GLuint getTextureColorId();

private:
  GLuint m_locEyePos;
  GLuint m_texIdColor;
};


#endif