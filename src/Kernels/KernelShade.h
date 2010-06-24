#ifndef KernelShade_H
#define KernelShade_H

#include "Kernels/KernelBase.h"

class KernelShade : public KernelBase {

public:
	KernelShade();
  KernelShade(int width, int height,  GLuint texIdRayDir, GLuint texIdTriangleHitInfo, GLuint texIdvertexes, GLuint texIdNormals, 
    GLuint texIdDiffuseTex, GLuint texIdSpecularTex, GLuint texIdLights, 
    GLfloat normalsTexSize, GLfloat vertexesTexSize, GLfloat diffuseTexSize, GLfloat specularTexSize,
    GLfloat lightsTexSize);
	~KernelShade();

	void step(Vector3 eyePos);
	GLuint getTextureColorId();

private:
  GLuint m_locEyePos;
  GLuint m_texIdColor;
};


#endif