#include "KernelMng.h"


KernelMng::KernelMng(int width, int height, UniformGrid* uniformGrid){
	m_kernelGenerateRay = KernelGenerateRay(width, height, uniformGrid->getBBMin(), uniformGrid->getBBMax());

	m_kernelTraverse = KernelTraverse(width, height, uniformGrid->getVoxelSize(),
										uniformGrid->getGridTextureId(),
										m_kernelGenerateRay.getTexIdRayPos(),
										m_kernelGenerateRay.getTexIdRayDir());

	m_kernelIntersect = KernelIntersect();
	m_kernelShade = KernelShade();
	m_currentState = GENERATERAY;
	m_uniformGrid = uniformGrid;

}


void KernelMng::step(Vector3 eyePos, Vector3 eyeDir){
	render(eyePos, eyeDir);
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

void KernelMng::render(Vector3 eyePos, Vector3 eyeDir){

	switch(m_currentState){
		case GENERATERAY:
			m_kernelGenerateRay.step(eyePos, eyeDir);
			break;
		case TRAVERSE:
			m_kernelTraverse.step();
			break;
		case INTERSECT:
			m_kernelIntersect.step();
			break;
		default:
			m_kernelShade.step();
			break;
	}

}

KernelMngState KernelMng::oracle(){

	return INTERSECT;
}

int KernelMng::countActiveRays(){
	return 0;
}