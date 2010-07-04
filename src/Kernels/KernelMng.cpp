#include "KernelMng.h"


KernelMng::KernelMng(int width, int height,RTScene* scene, float nearPlaneWidth, float nearPlaneHeight){


	UniformGrid* uniformGrid = scene->getUniformGrid();

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
											m_kernelGenerateRay->getTexIdRayDir(), scene->getGridTexId(), scene->getTriangleListTexId(),
											scene->getVertexesTexId(), scene->getGridTexSize(), scene->getTriangleListTexSize(),
											scene->getVertexesTexSize());


	m_kernelShade = new KernelShade(width, height, m_kernelGenerateRay->getTexIdRayDir(),m_kernelGenerateRay->getTexIdRayPos(),
                  m_kernelIntersect->getTexIdTriangleHitInfo(), 
									scene->getVertexesTexId(), scene->getNormalsTexId(), scene->getMaterialTexId(),
                  scene->getLightsTexId(), scene->getNormalsTexSize(),
									scene->getVertexesTexSize(), scene->getMaterialTexSize(),
									scene->getLightsTexSize(), scene->getClearColor()
                  );
	
	m_currentState = GENERATERAY;
	m_numTraverses = 0;
	glGenQueries(1, &m_occlusionQueryId);
	//m_uniformGrid = uniformGrid;

}


void KernelMng::step(bool updateStates, int traversePerIntersection, KernelMngState stateToStop, Vector3 eyePos, Vector3 eyeDir, Vector3 eyeUp, Vector3 eyeRight, float nearPlane){
  cout << "IN:" << m_currentState ;
  render(eyePos, eyeDir, eyeUp, eyeRight, nearPlane);
  if(updateStates) update(traversePerIntersection, stateToStop);
  cout << " OUT:" << m_currentState <<endl;

}

GLuint KernelMng::getTextureColorId(){
	return m_kernelShade->getTextureColorId();
}

void KernelMng::update(int traversePerIntersection, KernelMngState stateToStop){

	if(m_currentState == stateToStop){
		m_currentState = GENERATERAY;	
		//return;
	}
	if(m_currentState == GENERATERAY){
		m_currentState = CALCULATEVOXEL;
	}
	else if(m_currentState == CALCULATEVOXEL){
		m_currentState = TRAVERSE;
		m_numTraverses++;
	}
	else if(m_currentState == INTERSECT){
		m_currentState = SHADE;
	}
	else if(m_currentState == TRAVERSE){
		m_numTraverses++;
	}
	//else if(m_currentState == SHADE){
		//m_currentState = TRAVERSE;
	//}
	if(m_currentState == INTERSECT || m_currentState == TRAVERSE){
		m_currentState = oracle(traversePerIntersection);
	}
}
void KernelMng::generateRay(){
	m_currentState = GENERATERAY;
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
		case CALCULATEVOXEL:
			m_kernelCalculateVoxel->step();
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

	if(m_currentState == INTERSECT){
		newState = TRAVERSE;
	}
	else{
		if(m_numTraverses > traversePerIntersection){
			newState = INTERSECT;
			m_numTraverses = 0;
		}
	}


	//std::cout << countActiveRays() << "\n";

	return newState;

}

int KernelMng::countActiveRays(){
	
	glEnable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);

	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_ZERO, GL_KEEP, GL_KEEP);
	glBeginQuery(GL_SAMPLES_PASSED_ARB, m_occlusionQueryId);
	m_kernelShade->step(Vector3(0, -150, 0));
	glEndQuery(GL_SAMPLES_PASSED_ARB);
	GLuint occ = 0;
	glGetQueryObjectuivARB(m_occlusionQueryId, GL_QUERY_RESULT_ARB, &occ);

	return occ;

}

KernelMngState KernelMng::getCurrentState() const
{
 return m_currentState; 
}

void KernelMng::setCurrentState( KernelMngState val )
{
  m_currentState = val;
}
