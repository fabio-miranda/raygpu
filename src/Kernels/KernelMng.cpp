#include "KernelMng.h"


KernelMng::KernelMng(int width, int height,RTScene* scene, float nearPlaneWidth, float nearPlaneHeight){

	m_numRays = width * height;

	UniformGrid* uniformGrid = scene->getUniformGrid();

	m_kernelCounter = new KernelCounter(width, height);

	m_kernelGenerateRay = new KernelGenerateRay(width, height, scene->getGridTexSize(), uniformGrid->getNumVoxels(), uniformGrid->getVoxelSize(), uniformGrid->getBBMin(), uniformGrid->getBBMax(), nearPlaneWidth, nearPlaneHeight);

	m_kernelCalculateVoxel = new KernelCalculateVoxel(width, height, uniformGrid->getVoxelSize(),
													  uniformGrid->getBBMin(),
													  uniformGrid->getBBMax(),
													  scene->getGridTexId(),
													  scene->getGridTexSize(),
													  uniformGrid->getNumVoxels(),
													  m_kernelGenerateRay->getTexIdRayPos(),
													  m_kernelGenerateRay->getTexIdRayDir());



	m_kernelTraverse = new KernelTraverse( width, height, uniformGrid->getVoxelSize(),
											uniformGrid->getBBMin(),
											uniformGrid->getBBMax(),
											scene->getGridTexId(),
											scene->getGridTexSize(),
											uniformGrid->getNumVoxels(),
											m_kernelGenerateRay->getTexIdRayPos(),
											m_kernelGenerateRay->getTexIdRayDir(),
											m_kernelCalculateVoxel->getTexIdIntersectionMax());


	m_kernelIntersect = new KernelIntersect(width, height, m_kernelGenerateRay->getTexIdRayPos(),
											m_kernelGenerateRay->getTexIdRayDir(),
                      m_kernelGenerateRay->getTexIdShadow(),
                      scene->getGridTexId(), scene->getTriangleListTexId(),
											scene->getVertexesTexId(), scene->getGridTexSize(), scene->getTriangleListTexSize(),
											scene->getVertexesTexSize());


	m_kernelShade = new KernelShade(width, height, m_kernelGenerateRay->getTexIdRayDir(),
                  m_kernelGenerateRay->getTexIdRayPos(), m_kernelGenerateRay->getTexIdColor(),
                  m_kernelIntersect->getTexIdTriangleHitInfo(), 
                  m_kernelGenerateRay->getTexIdShadow(),
									scene->getVertexesTexId(), scene->getNormalsTexId(), 
                  scene->getMaterialTexId(),
                  scene->getLightsTexId(), scene->getNormalsTexSize(),
									scene->getVertexesTexSize(), scene->getMaterialTexSize(),
									scene->getLightsTexSize(), scene->getClearColor()
                  );
	
	m_currentState = GENERATERAY;
	m_numTraverses = 0;
	m_countCalculateVoxel = 0;
	m_recursionLevel = 0;
	
	//m_uniformGrid = uniformGrid;

}

void KernelMng::stepCurrentState(int traversePerIntersection, Vector3 eyePos, Vector3 eyeDir, Vector3 eyeUp, Vector3 eyeRight, float nearPlane){

	cout << "IN:" << m_currentState ;
	render(m_currentState, eyePos, eyeDir, eyeUp, eyeRight, nearPlane);
	update(traversePerIntersection);
	cout << " OUT:" << m_currentState <<endl;

}

void KernelMng::stepState(KernelMngState stateToUpdate, Vector3 eyePos, Vector3 eyeDir, Vector3 eyeUp, Vector3 eyeRight, float nearPlane){
	
	std::cout << "==========\n";

	cout << "IN:" << stateToUpdate ;
	render(stateToUpdate, eyePos, eyeDir, eyeUp, eyeRight, nearPlane);
	cout << " OUT:" << stateToUpdate <<endl;

	countActiveRays();

	std::cout << "==========\n";

}

GLuint KernelMng::getTextureColorId(){
	return m_kernelGenerateRay->getTexIdColor();
}

void KernelMng::update(int traversePerIntersection){
	/*
	if(m_currentState == stateToStop){
		m_currentState = GENERATERAY;	
		//return;
	}
	*/
	if(m_currentState == GENERATERAY){
		m_currentState = CALCULATEVOXEL;
	}
	else if(m_currentState == CALCULATEVOXEL){
		m_currentState = TRAVERSE;
	}
	else{
		m_currentState = oracle(traversePerIntersection);
	}
}
void KernelMng::generateRay(){
	m_currentState = GENERATERAY;
}

void KernelMng::render(KernelMngState state, Vector3 eyePos, Vector3 eyeDir, Vector3 eyeUp, Vector3 eyeRight, float nearPlane){

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	switch(state){
		case GENERATERAY:
			m_kernelGenerateRay->step(eyePos, eyeDir, eyeUp, eyeRight, nearPlane);
			break;
		case CALCULATEVOXEL:
			m_kernelCalculateVoxel->step();
			break;
		case TRAVERSE:
			m_kernelTraverse->step();
			break;
		case INTERSECT:
			m_kernelIntersect->step();
			break;
		case SHADE:
			m_kernelShade->step(eyePos);
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

void KernelMng::renderKernelOutput(bool renderCurrentState, KernelMngState stateToRender, int outputNum){

	if(renderCurrentState)
		stateToRender = m_currentState;

	GLuint textureId;
	if(stateToRender == GENERATERAY)
		textureId = m_kernelGenerateRay->getOutputTexture(outputNum);
	else if(stateToRender == CALCULATEVOXEL)
		textureId = m_kernelCalculateVoxel->getOutputTexture(outputNum);
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

  //glEnable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glColor3f(1,1,1);
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);glVertex3f(0,0,0); // glColor3f(1, 1, 0);
		glTexCoord2f(1,0);glVertex3f(1,0,0);// glColor3f(1, 0, 0);
		glTexCoord2f(1,1);glVertex3f(1,1,0); // glColor3f(1, 0, 0);
		glTexCoord2f(0,1);glVertex3f(0,1,0); // glColor3f(1, 0, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
}

KernelMngState KernelMng::oracle(int traversePerIntersection){
	

	KernelMngState newState = m_currentState;

	int countInactive = m_kernelCounter->count(m_kernelGenerateRay->getTexIdRayDir(), 0);
	int countTraverse = m_kernelCounter->count(m_kernelGenerateRay->getTexIdRayDir(), 2);
	int countTraverseSec = m_kernelCounter->count(m_kernelGenerateRay->getTexIdRayDir(), 4);
	int countIntersect = m_kernelCounter->count(m_kernelGenerateRay->getTexIdRayDir(), 3);
	int countCalculateVoxel = m_kernelCounter->count(m_kernelGenerateRay->getTexIdRayDir(), 1);
	int countShading = m_kernelCounter->count(m_kernelGenerateRay->getTexIdRayDir(), 5);


	if(countIntersect > 0.2 * countTraverse){
		newState = INTERSECT;
	}
	else if(countTraverse > 0 || countTraverseSec > 0){
		newState = TRAVERSE;
	}
	else if(countCalculateVoxel > 0){
		if(m_currentState == SHADE)
			newState = CALCULATEVOXEL;
		else
			newState = SHADE;
	}
	else if(countShading > 0)
		newState = SHADE;

	

	m_countCalculateVoxel = countCalculateVoxel;

	//countActiveRays();
	

	return newState;

}

int KernelMng::countActiveRays(){

	std::cout << "Inactive: " << m_kernelCounter->count(m_kernelGenerateRay->getTexIdRayDir(), 0) << "\n";
	std::cout << "Active calculate voxel: " << m_kernelCounter->count(m_kernelGenerateRay->getTexIdRayDir(), 1) << "\n";
	std::cout << "Active traverse: " << m_kernelCounter->count(m_kernelGenerateRay->getTexIdRayDir(), 2) << "\n";
	std::cout << "Active intersect: " << m_kernelCounter->count(m_kernelGenerateRay->getTexIdRayDir(), 3) << "\n";
	std::cout << "Active traverse sec: " << m_kernelCounter->count(m_kernelGenerateRay->getTexIdRayDir(), 4) << "\n";
	std::cout << "Active shading: " << m_kernelCounter->count(m_kernelGenerateRay->getTexIdRayDir(), 5) << "\n";
	std::cout << "Overflow: " << m_kernelCounter->count(m_kernelGenerateRay->getTexIdRayDir(), 6) << "\n";
	std::cout << "Done: " << m_kernelCounter->count(m_kernelGenerateRay->getTexIdRayDir(), 7) << "\n";

	return 0;

	//return 0;
	//m_kernelCounter->step();

}

KernelMngState KernelMng::getCurrentState() const
{
 return m_currentState; 
}

void KernelMng::setCurrentState( KernelMngState val )
{
  m_currentState = val;
}
