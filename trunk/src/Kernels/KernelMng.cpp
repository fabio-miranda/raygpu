#include "KernelMng.h"


KernelMng::KernelMng(int width, int height,RTScene* scene){


	UniformGrid* uniformGrid = scene->GetUniformGrid();

	m_kernelGenerateRay = KernelGenerateRay(width, height, uniformGrid->getBBMin(), uniformGrid->getBBMax());

	m_kernelTraverse = KernelTraverse(width, height, uniformGrid->getVoxelSize(),
										scene->getGridTexId(),
										//scene->getGridTexSize(),
										Vector3(0,0,0),
										m_kernelGenerateRay.getTexIdRayPos(),
										m_kernelGenerateRay.getTexIdRayDir());

	m_kernelIntersect = KernelIntersect(width, height, m_kernelGenerateRay.getTexIdRayPos(),
										m_kernelGenerateRay.getTexIdRayDir(), scene->getGridTexId(), scene->getTriangleListTexId(),
										scene->getVertexesTexId(), scene->getGridTexSize(), scene->getTriangleListTexSize(),
										scene->getVertexesTexSize());

	m_kernelShade = KernelShade(width, height, m_kernelIntersect.getTexIdTriangleHitInfo(), 
								scene->getVertexesTexId(), scene->getNormalsTexId(), scene->getDiffuseTexId(),
								scene->getSpecularTexId(), scene->getLightsTexId(), scene->getNormalsTexSize(),
								scene->getVertexesTexSize(), scene->getDiffuseTexSize(), scene->getSpecularTexSize(),
								scene->getLightsTexSize());
	//m_currentState = GENERATERAY;
	//m_uniformGrid = uniformGrid;

}


void KernelMng::step(Vector3 eyePos, Vector3 eyeDir, Vector3 eyeUp, Vector3 eyeRight, float nearPlane){
	render(eyePos, eyeDir, eyeUp, eyeRight, nearPlane);
	update();	
}

void KernelMng::update(){
	
	if(m_currentState == GENERATERAY){
		m_currentState = TRAVERSE;
	}
	else if(m_currentState == INTERSECT || m_currentState == TRAVERSE){

		if(countActiveRays() > 0){
			m_currentState = oracle();
		}
		else
			m_currentState = SHADE;
	}

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
			m_kernelGenerateRay.step(eyePos, eyeDir, eyeUp, eyeRight, nearPlane);
			break;
		case TRAVERSE:
			m_kernelTraverse.step();
			break;
		case INTERSECT:
			m_kernelIntersect.step();
			break;
		default:
			m_kernelShade.step(eyePos);
			break;
	}

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