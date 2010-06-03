#include "KernelMng.h"


KernelMng::KernelMng(int width, int height,RTScene* scene){
  UniformGrid* uniformGrid = scene->GetUniformGrid();

	//m_kernelGenerateRay = KernelGenerateRay(width, height, uniformGrid->getBBMin(), uniformGrid->getBBMax());

	//m_kernelTraverse = KernelTraverse(width, height, uniformGrid->getVoxelSize(),
	//									scene->getGridTexId(),
	//									m_kernelGenerateRay.getTexIdRayPos(),
	//									m_kernelGenerateRay.getTexIdRayDir());

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
			m_kernelShade.step(eyePos);
			break;
	}

}

KernelMngState KernelMng::oracle(){

	return INTERSECT;
}

int KernelMng::countActiveRays(){
	return 0;
}