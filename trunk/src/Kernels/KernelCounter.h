#ifndef KernelCounter_H
#define KernelCounter_H


#include "Kernels/KernelBase.h"
#include "SpacePartition/UniformGrid.h"
#include "GraphBasis/Shader.h"
#include "GraphBasis/Vector3.h"

class KernelCounter : public KernelBase {

public:
	KernelCounter();
	KernelCounter(int width, int height);
	~KernelCounter();

	void step(GLuint texId, int state);
	int count(GLuint texId, int state);

private:
	GLuint m_texIdCount;
	GLuint m_occlusionQueryId;
	GLuint m_locState;

};


#endif