#include "KernelMng.h"


KernelMng::KernelMng(int width, int height,RTScene* scene){


	UniformGrid* uniformGrid = scene->GetUniformGrid();

	m_kernelGenerateRay = new KernelGenerateRay(width, height, uniformGrid->getBBMin(), uniformGrid->getBBMax());

	m_kernelTraverse = new KernelTraverse(width, height, uniformGrid->getVoxelSize(),
											scene->getGridTexId(),
											scene->getGridTexSize(),
											uniformGrid->getNumVoxels(),
											m_kernelGenerateRay->getTexIdRayPos(),
											m_kernelGenerateRay->getTexIdRayDir());

	m_kernelIntersect = new KernelIntersect(width, height, m_kernelGenerateRay->getTexIdRayPos(),
											m_kernelGenerateRay->getTexIdRayDir(), scene->getGridTexId(), scene->getTriangleListTexId(),
											scene->getVertexesTexId(), scene->getGridTexSize(), scene->getTriangleListTexSize(),
											scene->getVertexesTexSize());

	m_kernelShade = new KernelShade(width, height, m_kernelGenerateRay->getTexIdRayDir(), m_kernelIntersect->getTexIdTriangleHitInfo(), 
									scene->getVertexesTexId(), scene->getNormalsTexId(), scene->getDiffuseTexId(),
									scene->getSpecularTexId(), scene->getLightsTexId(), scene->getNormalsTexSize(),
									scene->getVertexesTexSize(), scene->getDiffuseTexSize(), scene->getSpecularTexSize(),
									scene->getLightsTexSize());
	
	m_currentState = GENERATERAY;
	//m_uniformGrid = uniformGrid;

}


void KernelMng::step(KernelMngState stateToStop, Vector3 eyePos, Vector3 eyeDir, Vector3 eyeUp, Vector3 eyeRight, float nearPlane){
	render(eyePos, eyeDir, eyeUp, eyeRight, nearPlane);
	update(stateToStop);	
}

GLuint KernelMng::getTextureColorId(){
	return m_kernelShade->getTextureColorId();
}

void KernelMng::update(KernelMngState stateToStop){
	
	KernelMngState newState;
	if(m_currentState == GENERATERAY){
			newState = TRAVERSE;
	}
	else if(m_currentState == INTERSECT || m_currentState == TRAVERSE){

		if(countActiveRays() > 0){
			newState = oracle();
		}
		else
			newState = SHADE;
	}

	if(m_currentState != stateToStop)
		m_currentState = newState;
}

void KernelMng::render(Vector3 eyePos, Vector3 eyeDir, Vector3 eyeUp, Vector3 eyeRight, float nearPlane){

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	switch(m_currentState){
		case GENERATERAY:
			m_kernelGenerateRay->step(eyePos, eyeDir, eyeUp, eyeRight, nearPlane);
			break;
		case TRAVERSE:
			m_kernelTraverse->step();
			break;
		case INTERSECT:
			m_kernelIntersect->step();
			break;
		default:
			m_kernelShade->step(eyePos);
			break;
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	
	
}

void KernelMng::renderKernelOutput(KernelMngState stateToRender, int outputNum){

	GLuint textureId;
	if(stateToRender == GENERATERAY)
		textureId = m_kernelGenerateRay->getOutputTexture(outputNum);
	else if(stateToRender == TRAVERSE)
		textureId = m_kernelTraverse->getOutputTexture(outputNum);
	else if(stateToRender == INTERSECT)
		textureId = m_kernelIntersect->getOutputTexture(outputNum);
	else if(stateToRender == SHADE)
		textureId = m_kernelShade->getOutputTexture(outputNum);

	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glBindTexture(GL_TEXTURE_2D,textureId);
	glBegin(GL_QUADS);
		glVertex3f(0,0,0); glTexCoord2f(0,0);// glColor3f(1, 0, 0);
		glVertex3f(1,0,0); glTexCoord2f(1,0);// glColor3f(1, 0, 0);
		glVertex3f(1,1,0); glTexCoord2f(1,1);// glColor3f(1, 0, 0);
		glVertex3f(0,1,0); glTexCoord2f(0,1);// glColor3f(1, 0, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
}

KernelMngState KernelMng::oracle(){

	if(m_currentState == INTERSECT)
		return TRAVERSE;
	else if(m_currentState == TRAVERSE)
		return INTERSECT;
}

int KernelMng::countActiveRays(){
	return 0;
}