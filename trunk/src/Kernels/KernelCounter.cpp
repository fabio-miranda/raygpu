#include "KernelCounter.h"


KernelCounter::KernelCounter(){


}

KernelCounter::KernelCounter(int width, int height)
: KernelBase("./resources/vertice.vert", "./resources/counter.frag", width, height){

	glGenQueries(1, &m_occlusionQueryId);

	//Output
	m_texIdCount = addOutput(0);

	//Input
	m_shader->setActive(true);
		addInputTexture(GL_TEXTURE_2D, "samplerTex", 0);
		m_locState = addInputFloat("state", 0);
	m_shader->setActive(false);

}

KernelCounter::~KernelCounter(){

}

void KernelCounter::step(GLuint texId, int state){

	m_fbo->setActive(true);
	m_shader->setActive(true);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glClearColor(0, 0, 0, 0);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glActiveTextureARB(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texId);

	glUniform1f(m_locState, state);
	
	renderQuad();
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	
	m_shader->setActive(false);
	m_fbo->setActive(false);

}

int KernelCounter::count(GLuint texId, int state){

	//glEnable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
	//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	//glDisable(GL_BLEND);


	//glStencilFunc(GL_EQUAL, 1, 1);
	//glStencilOp(GL_ZERO, GL_KEEP, GL_KEEP);
	glBeginQuery(GL_SAMPLES_PASSED_ARB, m_occlusionQueryId);

	step(texId, state);

	glEndQuery(GL_SAMPLES_PASSED_ARB);
	GLuint occ = 0;
	glGetQueryObjectuivARB(m_occlusionQueryId, GL_QUERY_RESULT_ARB, &occ);


	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glDisable(GL_STENCIL_TEST);

	//std::cout << occ << "\n";

	return occ;

}