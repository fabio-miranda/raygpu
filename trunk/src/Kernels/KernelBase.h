#ifndef KernelBase_H
#define KernelBase_H

#include <vector>

#include <GraphBasis/FrameBufferObject.h>
#include <GraphBasis/Vector3.h>
#include <GraphBasis/Shader.h>

class KernelBase {

public:
	KernelBase();
	KernelBase(char* frag, char* vert, int width, int height);
	~KernelBase();
	void step();

protected:
	void renderQuad();
	void activateTextures();
	GLuint addInputTexture(char* name, GLuint id);
	GLuint addInputVec3(char* name, Vector3 value);
	GLuint addOutput(int index, GLuint textureId = -1);

	FrameBufferObject m_fbo;
	Shader m_shader;
	std::vector<GLuint> m_inputTextures;
};


#endif