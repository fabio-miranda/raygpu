#include "UniformGrid.h"
#include <math.h>

UniformGrid::UniformGrid(unsigned int p_numTriangles, std::vector<RTMesh>* p_mesh, 
                         std::vector<RTMaterial>* p_material, std::vector<RTLight>* p_light, 
                         Vector3 p_numVoxels, Color clearColor)
:m_clearColor(clearColor)
{
	m_min = Vector3(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
	m_max = Vector3(0, 0, 0);

	calculateBB(p_mesh, p_numVoxels);
	calculateGrid(p_numTriangles, p_mesh, p_material, p_light, p_numVoxels);
}

UniformGrid::UniformGrid( string rtbFileName )
{
  m_min = Vector3(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
  m_max = Vector3(0, 0, 0);

  readRTBFile(rtbFileName);
}


UniformGrid::~UniformGrid(){

	delete [] m_gridArray;
	delete [] m_triangleListArray;
	delete [] m_triangleVertexArray;
	delete [] m_triangleNormalsArray;
	delete [] m_triangleMaterialArray;
}
	
void UniformGrid::calculateBB(std::vector<RTMesh>* p_mesh, Vector3 p_numVoxels)
{

	std::vector<RTMesh> :: iterator meshIt;
	for(meshIt = p_mesh->begin(); meshIt!=p_mesh->end(); ++meshIt){
		std::vector<RTTriangle>* trianglesArray = meshIt->getTriangles();
		for(int i=0; i<trianglesArray->size(); i++){
			RTTriangle currentTriangle = trianglesArray->at(i);

			setMinMax(currentTriangle.v1);
			setMinMax(currentTriangle.v2);
			setMinMax(currentTriangle.v3);
		}
	}

	m_numVoxels = Vector3(p_numVoxels);
	m_gridSize = Vector3(fabsf(m_max.x - m_min.x), fabsf(m_max.y - m_min.y), fabsf(m_max.z - m_min.z));
	m_voxelSize = Vector3(m_gridSize.x / p_numVoxels.x, m_gridSize.y / p_numVoxels.y, m_gridSize.z / p_numVoxels.z);
}

void UniformGrid::calculateGrid(unsigned int p_numTriangles, std::vector<RTMesh>* p_mesh, std::vector<RTMaterial>* p_material, std::vector<RTLight>* p_light, Vector3 p_numVoxels)
{
	std::vector<RTTriangle*>* aux_grid = new std::vector<RTTriangle*>[(int)(p_numVoxels.x * p_numVoxels.y * p_numVoxels.z)];
  
	unsigned int size = 0;
	for(unsigned int i=0; i<p_mesh->size(); i++){
		std::vector<RTTriangle>* trianglesArray = p_mesh->at(i).getTriangles();
		for(unsigned int j=0; j<trianglesArray->size(); j++){
			RTTriangle* currentTriangle = &(trianglesArray->at(j));

			Vector3 voxelIndex_vertex1 = getVertexGridIndex(currentTriangle->v1);
			Vector3 voxelIndex_vertex2 = getVertexGridIndex(currentTriangle->v2);
			Vector3 voxelIndex_vertex3 = getVertexGridIndex(currentTriangle->v3);

			aux_grid[getVoxelAt(voxelIndex_vertex1)].push_back(currentTriangle);
			size++;
			if((voxelIndex_vertex2 == voxelIndex_vertex1) == false){
				aux_grid[getVoxelAt(voxelIndex_vertex2)].push_back(currentTriangle);
				size++;
			}
			if((voxelIndex_vertex3 == voxelIndex_vertex2) == false && (voxelIndex_vertex3 == voxelIndex_vertex1) == false){
				aux_grid[getVoxelAt(voxelIndex_vertex3)].push_back(currentTriangle);
				size++;
			}

			
			//Test triangle / box intersections
			for(float x=0; x<m_numVoxels.x; x++){
				for(float y=0; y<m_numVoxels.y; y++){
					for(float z=0; z<m_numVoxels.z; z++){
						Vector3 bbMin = m_min + Vector3(x * m_voxelSize.x, y * m_voxelSize.y, z * m_voxelSize.z);
						Vector3 bbMax = bbMin + m_voxelSize;
						

						float bbCenter [] = {bbMin.x + ((bbMax.x - bbMin.x) / 2.0f),
											(bbMin.y + (bbMax.y - bbMin.y) / 2.0f),
											(bbMin.z + (bbMax.z - bbMin.z) / 2.0f)};

						float bbHalfSize [] = {((m_voxelSize).x / 2.0f),
												((m_voxelSize).y / 2.0f),
												((m_voxelSize).z / 2.0f)};

						float triangle [][3] = {{currentTriangle->v1.x, currentTriangle->v1.y, currentTriangle->v1.z},
												 {currentTriangle->v2.x, currentTriangle->v2.y, currentTriangle->v2.z},
												 {currentTriangle->v3.x, currentTriangle->v3.y, currentTriangle->v3.z}};

						bool hit = triBoxOverlap(bbCenter, bbHalfSize, triangle);
						//std::cout << triBoxOverlap(bbCenter, bbHalfSize, triangle) << "\n";

						Vector3 voxel = Vector3(x,y,z);
						if(hit == true && (voxel == voxelIndex_vertex1) == false
							&& (voxel == voxelIndex_vertex2) == false && (voxel == voxelIndex_vertex3) == false){
							aux_grid[getVoxelAt(voxel)].push_back(currentTriangle);
							size++;
						}
					}
				}
			}
		}
	}
 // 
  GLint max_tex_size = 0;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);
  int texture2DnumLines;

  
	m_gridArraySize = (int)(p_numVoxels.x * p_numVoxels.y * p_numVoxels.z) * 4;
  texture2DnumLines = (int)((m_gridArraySize/4)/max_tex_size) + (int)(m_gridArraySize%max_tex_size != 0);
	m_gridArray = new GLfloat[texture2DnumLines*max_tex_size*4];
	for(int i = 0; i < texture2DnumLines*max_tex_size*4; ++i) m_gridArray[i] = -1.0f;
	//memset(m_gridArray, -1.0f, sizeof(GLfloat) * m_gridArraySize);//Do not work for floats different then 0 

	size += p_numVoxels.x * p_numVoxels.y * p_numVoxels.z;
	m_triangleListArraySize = size;
  texture2DnumLines = (int)((m_triangleListArraySize)/max_tex_size) + (int)(m_triangleListArraySize%max_tex_size != 0);
	m_triangleListArray = new GLfloat[texture2DnumLines*max_tex_size];
	for(int i = 0; i < texture2DnumLines*max_tex_size; ++i) m_triangleListArray[i] = -1.0f;

	m_triangleVertexArraySize = p_numTriangles * 3 * 3;
  texture2DnumLines = (int)((m_triangleVertexArraySize/3)/max_tex_size) + (int)(m_triangleVertexArraySize%max_tex_size != 0);
 	m_triangleVertexArray = new GLfloat[texture2DnumLines*max_tex_size*3];
	for(int i = 0; i < texture2DnumLines*max_tex_size*3; ++i) m_triangleVertexArray[i] = 1.0f;
  
	m_triangleNormalsArraySize = p_numTriangles * 3 * 3;
  texture2DnumLines = (int)((m_triangleNormalsArraySize/3)/max_tex_size) + (int)(m_triangleNormalsArraySize%max_tex_size != 0);
	m_triangleNormalsArray = new GLfloat[texture2DnumLines*max_tex_size*3];
	memset(m_triangleNormalsArray, 0, sizeof(GLfloat) * texture2DnumLines*max_tex_size*3);

	m_triangleMaterialArraySize = p_numTriangles * 3 * 4;
  texture2DnumLines = (int)((m_triangleMaterialArraySize/4)/max_tex_size) + (int)(m_triangleMaterialArraySize%max_tex_size != 0);
	m_triangleMaterialArray = new GLfloat[texture2DnumLines*max_tex_size*4];
	memset(m_triangleMaterialArray, 0, sizeof(GLfloat) * texture2DnumLines*max_tex_size*4);

  m_lightsArraySize = p_light->size()*sizeof(struct lightStruct)/(sizeof(GLfloat));
  m_lightsArray = new GLfloat[m_lightsArraySize];
  for( unsigned int i = 0; i < p_light->size(); ++i)
    memcpy(&m_lightsArray[i*sizeof(struct lightStruct)/sizeof(GLfloat)], p_light->at(i).getLightStruct(), sizeof(struct lightStruct));

 
	unsigned int gridCount = 0;
	unsigned int gridCountPlusOne = 0;
	int cont = 0;
	int gridIndexCont = 0;
	for(int x=0; x<p_numVoxels.x; x++){
		for(int y=0; y<p_numVoxels.y; y++){
			for(int z=0; z<p_numVoxels.z; z++){
				unsigned int prevGridCountPlusOne = gridCountPlusOne;
				for(int j=0; j<aux_grid[cont].size(); j++){
					unsigned int triangleIndex = aux_grid[cont].at(j)->getGlobalIndex();

					m_triangleListArray[gridCountPlusOne+j] = triangleIndex;


					m_triangleVertexArray[triangleIndex*3*3] = aux_grid[cont].at(j)->v1.x;
					m_triangleVertexArray[triangleIndex*3*3+1] = aux_grid[cont].at(j)->v1.y;
					m_triangleVertexArray[triangleIndex*3*3+2] = aux_grid[cont].at(j)->v1.z;

					m_triangleVertexArray[triangleIndex*3*3+3] = aux_grid[cont].at(j)->v2.x;
					m_triangleVertexArray[triangleIndex*3*3+3+1] = aux_grid[cont].at(j)->v2.y;
					m_triangleVertexArray[triangleIndex*3*3+3+2] = aux_grid[cont].at(j)->v2.z;

					m_triangleVertexArray[triangleIndex*3*3+3+3] = aux_grid[cont].at(j)->v3.x;
					m_triangleVertexArray[triangleIndex*3*3+3+3+1] = aux_grid[cont].at(j)->v3.y;
					m_triangleVertexArray[triangleIndex*3*3+3+3+2] = aux_grid[cont].at(j)->v3.z;



					m_triangleNormalsArray[triangleIndex*3*3] = aux_grid[cont].at(j)->n1.x;
					m_triangleNormalsArray[triangleIndex*3*3+1] = aux_grid[cont].at(j)->n1.y;
					m_triangleNormalsArray[triangleIndex*3*3+2] = aux_grid[cont].at(j)->n1.z;

          m_triangleNormalsArray[triangleIndex*3*3+3] = aux_grid[cont].at(j)->n2.x;
          m_triangleNormalsArray[triangleIndex*3*3+3+1] = aux_grid[cont].at(j)->n2.y;
          m_triangleNormalsArray[triangleIndex*3*3+3+2] = aux_grid[cont].at(j)->n2.z;

          m_triangleNormalsArray[triangleIndex*3*3+3+3] = aux_grid[cont].at(j)->n3.x;
          m_triangleNormalsArray[triangleIndex*3*3+3+3+1] = aux_grid[cont].at(j)->n3.y;
          m_triangleNormalsArray[triangleIndex*3*3+3+3+2] = aux_grid[cont].at(j)->n3.z;



          m_triangleMaterialArray[triangleIndex*3*4] = p_material->at(aux_grid[cont].at(j)->getMaterialIndex()).opacity;
          m_triangleMaterialArray[triangleIndex*3*4+1] = p_material->at(aux_grid[cont].at(j)->getMaterialIndex()).reflective;
          m_triangleMaterialArray[triangleIndex*3*4+2] = p_material->at(aux_grid[cont].at(j)->getMaterialIndex()).refractive;
          m_triangleMaterialArray[triangleIndex*3*4+3] = 1.0;

          m_triangleMaterialArray[triangleIndex*3*4+4] = p_material->at(aux_grid[cont].at(j)->getMaterialIndex()).diffuse.r;
					m_triangleMaterialArray[triangleIndex*3*4+4+1] = p_material->at(aux_grid[cont].at(j)->getMaterialIndex()).diffuse.g;
					m_triangleMaterialArray[triangleIndex*3*4+4+2] = p_material->at(aux_grid[cont].at(j)->getMaterialIndex()).diffuse.b;
          m_triangleMaterialArray[triangleIndex*3*4+4+3] = 1.0;

          m_triangleMaterialArray[triangleIndex*3*4+4+4] = p_material->at(aux_grid[cont].at(j)->getMaterialIndex()).specular.r;
          m_triangleMaterialArray[triangleIndex*3*4+4+4+1] = p_material->at(aux_grid[cont].at(j)->getMaterialIndex()).specular.g;
          m_triangleMaterialArray[triangleIndex*3*4+4+4+2] = p_material->at(aux_grid[cont].at(j)->getMaterialIndex()).specular.b;
          m_triangleMaterialArray[triangleIndex*3*4+4+4+3] = p_material->at(aux_grid[cont].at(j)->getMaterialIndex()).specularExp;
				}
				m_gridArray[gridIndexCont++] = x;
				m_gridArray[gridIndexCont++] = y;
				m_gridArray[gridIndexCont++] = z;
				m_gridArray[gridIndexCont++] = aux_grid[cont].size() != 0 ? (float)prevGridCountPlusOne : -1.0;
				
				gridCountPlusOne += aux_grid[cont].size()+1;
				gridCount += aux_grid[cont].size();

				cont++;
			}
		}
	}
	delete [] aux_grid;
}

Vector3 UniformGrid::getVertexGridIndex(Vector3 vertex){

	Vector3 index;
	index.x = (int)(((vertex.x - m_min.x-0.0001)/m_gridSize.x) * m_numVoxels.x);
	index.y = (int)(((vertex.y - m_min.y-0.0001)/m_gridSize.y) * m_numVoxels.y);
	index.z = (int)(((vertex.z - m_min.z-0.0001)/m_gridSize.z) * m_numVoxels.z);


	return index;
}

unsigned int UniformGrid::getVoxelAt(Vector3 index){

	return index.x*m_numVoxels.y*m_numVoxels.z + index.y*m_numVoxels.z + index.z;
}

void UniformGrid::setMinMax(Vector3 vertex){

	if(vertex.x < m_min.x)
		m_min.x = vertex.x;
	else if(vertex.x > m_max.x)
		m_max.x = vertex.x;

	if(vertex.y < m_min.y)
		m_min.y = vertex.y;
	else if(vertex.y > m_max.y)
		m_max.y = vertex.y;

	if(vertex.z < m_min.z)
		m_min.z = vertex.z;
	else if(vertex.z > m_max.z)
		m_max.z = vertex.z;

}

void UniformGrid::render(){

	
	for(int i=0; i<m_numVoxels.x; i++){
		//for(int j=0; j<1; j++){
      for(int j=0; j<m_numVoxels.y; j++){
			//for(int k=0; k<1; k++){
        for(int k=0; k<m_numVoxels.z; k++){
				
				
				//if(m_gridArray[getVoxelAt(Vector3(i, j, k))] > 0)
        {
					glPushMatrix();
					glTranslatef(m_min.x + i * m_voxelSize.x, m_min.y + j * m_voxelSize.y, m_min.z + k * m_voxelSize.z);
					glBegin(GL_LINES);
						glVertex3f(0, 0, 0); glVertex3f(0, 0, m_voxelSize.z);
						glVertex3f(0, 0, 0); glVertex3f(0, m_voxelSize.y, 0);
						glVertex3f(0, 0, 0); glVertex3f(m_voxelSize.x, 0, 0);

						glVertex3f(m_voxelSize.x, m_voxelSize.y, m_voxelSize.z); glVertex3f(m_voxelSize.x, m_voxelSize.y, 0);
						glVertex3f(m_voxelSize.x, m_voxelSize.y, m_voxelSize.z); glVertex3f(m_voxelSize.x, 0, m_voxelSize.z);
						glVertex3f(m_voxelSize.x, m_voxelSize.y, m_voxelSize.z); glVertex3f(0, m_voxelSize.y, m_voxelSize.z);
						
						glVertex3f(m_voxelSize.x, 0, m_voxelSize.z); glVertex3f(m_voxelSize.x, 0, 0);
						glVertex3f(0, m_voxelSize.y, m_voxelSize.z); glVertex3f(0, m_voxelSize.y, 0);
						
						glVertex3f(m_voxelSize.x, 0, m_voxelSize.z); glVertex3f(0, 0, m_voxelSize.z);
						glVertex3f(0, m_voxelSize.y, 0); glVertex3f(m_voxelSize.x, m_voxelSize.y, 0);
						

						glVertex3f(0, 0, m_voxelSize.z); glVertex3f(0, m_voxelSize.y, m_voxelSize.z);
						glVertex3f(m_voxelSize.x, 0, 0); glVertex3f(m_voxelSize.x, m_voxelSize.y, 0);
					glEnd();
					glPopMatrix();
				}
			}
		}
	}
	

}


Vector3 UniformGrid::getBBMin(){
	return m_min;
}
Vector3 UniformGrid::getBBMax(){
	return m_max;
}


Vector3 UniformGrid::getVoxelSize(){
  return m_voxelSize;
}
GLfloat* UniformGrid::getGridArray()
{
  return m_gridArray;
}

GLfloat* UniformGrid::getTriangleListArray()
{
  return m_triangleListArray;
}

GLfloat* UniformGrid::getTriangleVertexArray()
{
  return m_triangleVertexArray;
}

GLfloat* UniformGrid::getTriangleNormalsArray()
{
  return m_triangleNormalsArray;
}

GLfloat* UniformGrid::getTriangleMaterialArray()
{
  return m_triangleMaterialArray;
}

GLfloat* UniformGrid::getLightsArray()
{
  return m_lightsArray;
}


int UniformGrid::getGridArraySize(){
	return (int)((float)(m_gridArraySize)/4.0);
}

int UniformGrid::getTriangleListArraySize(){
	return m_triangleListArraySize;
}

int UniformGrid::getTriangleVertexArraySize(){
  return (int)((float)(m_triangleVertexArraySize)/3.0);
}

int UniformGrid::getTriangleMaterialArraySize(){
  return (int)((float)(m_triangleMaterialArraySize)/4.0);
}


int UniformGrid::getLightsArraySize()
{
  return (int)((float)(m_lightsArraySize)/4.0);
}
Vector3 UniformGrid::getNumVoxels(){
	return m_numVoxels;
}

int UniformGrid::getTriangleNormalsArraySize()
{
  return (int)((float)(m_triangleNormalsArraySize)/3.0);
}


int UniformGrid::getGridArrayAbsoluteSize()
{
  return m_gridArraySize;
}

int UniformGrid::getTriangleListArrayAbsoluteSize()
{
  return m_triangleListArraySize;
}

int UniformGrid::getTriangleVertexArrayAbsoluteSize()
{
  return m_triangleVertexArraySize;
}

int UniformGrid::getTriangleNormalsArrayAbsoluteSize()
{
  return m_triangleNormalsArraySize;
}

int UniformGrid::getTriangleMaterialArrayAbsoluteSize()
{
  return m_triangleMaterialArraySize;
}


int UniformGrid::getLightsArrayAbsoluteSize()
{
  return m_lightsArraySize;
}

struct fileBuffer
{
  GLint max_tex_size;
  float min_x, min_y, min_z;
  float max_x, max_y, max_z;
  float gridSize_x, gridSize_y, gridSize_z;
  float numVoxels_x, numVoxels_y, numVoxels_z;
  float voxelSize_x, voxelSize_y, voxelSize_z;
  float clearColor_r, clearColor_g, clearColor_b;

  int gridArraySize;
  int lightsArraySize;

  int triangleVertexArraySize;
  int triangleListArraySize;
  int triangleNormalsArraySize;
  int triangleMaterialArraySize;
};

void UniformGrid::writeRTBFile( string fileName )
{
  FILE *fp = fopen(fileName.c_str(), "wb");
  assert(fp);

  GLint max_tex_size = 0;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);
    
  struct fileBuffer writeFileBuffer;
  writeFileBuffer.max_tex_size = max_tex_size;
  writeFileBuffer.min_x = m_min.x;
  writeFileBuffer.min_y = m_min.y;
  writeFileBuffer.min_z = m_min.z;

  writeFileBuffer.max_x = m_max.x;
  writeFileBuffer.max_y = m_max.y;
  writeFileBuffer.max_z = m_max.z;

  writeFileBuffer.gridSize_x = m_gridSize.x;
  writeFileBuffer.gridSize_y = m_gridSize.y;
  writeFileBuffer.gridSize_z = m_gridSize.z;

  writeFileBuffer.numVoxels_x = m_numVoxels.x;
  writeFileBuffer.numVoxels_y = m_numVoxels.y;
  writeFileBuffer.numVoxels_z = m_numVoxels.z;

  writeFileBuffer.voxelSize_x = m_voxelSize.x;
  writeFileBuffer.voxelSize_y = m_voxelSize.y;
  writeFileBuffer.voxelSize_z = m_voxelSize.z;

  writeFileBuffer.clearColor_r = m_clearColor.r;
  writeFileBuffer.clearColor_g = m_clearColor.g;
  writeFileBuffer.clearColor_b = m_clearColor.b;

  writeFileBuffer.gridArraySize = m_gridArraySize;
  writeFileBuffer.lightsArraySize = m_lightsArraySize;

  writeFileBuffer.triangleVertexArraySize = m_triangleVertexArraySize;
  writeFileBuffer.triangleListArraySize = m_triangleListArraySize;
  writeFileBuffer.triangleNormalsArraySize = m_triangleNormalsArraySize;
  writeFileBuffer.triangleMaterialArraySize = m_triangleMaterialArraySize;

  fwrite(&writeFileBuffer, sizeof(fileBuffer),1,fp);

  int texture2DnumLines = (int)((m_gridArraySize/4)/max_tex_size) + (int)(m_gridArraySize%max_tex_size != 0);
  fwrite(m_gridArray, sizeof(GLfloat),texture2DnumLines*max_tex_size*4,fp);
  fwrite(m_lightsArray, sizeof(GLfloat),m_lightsArraySize,fp);
  
  texture2DnumLines = (int)((m_triangleVertexArraySize/3)/max_tex_size) + (int)(m_triangleVertexArraySize%max_tex_size != 0);
  fwrite(m_triangleVertexArray, sizeof(GLfloat),texture2DnumLines*max_tex_size*3,fp);

  texture2DnumLines = (int)((m_triangleListArraySize)/max_tex_size) + (int)(m_triangleListArraySize%max_tex_size != 0);
  fwrite(m_triangleListArray, sizeof(GLfloat),texture2DnumLines*max_tex_size,fp);

  texture2DnumLines = (int)((m_triangleNormalsArraySize/3)/max_tex_size) + (int)(m_triangleNormalsArraySize%max_tex_size != 0);
  fwrite(m_triangleNormalsArray, sizeof(GLfloat),texture2DnumLines*max_tex_size*3,fp);

  texture2DnumLines = (int)((m_triangleMaterialArraySize/4)/max_tex_size) + (int)(m_triangleMaterialArraySize%max_tex_size != 0);
  fwrite(m_triangleMaterialArray, sizeof(GLfloat),texture2DnumLines*max_tex_size*4,fp);

  fclose(fp);
}

void UniformGrid::readRTBFile( string fileName )
{
  FILE *fp = fopen(fileName.c_str(), "rb");
  assert(fp);

  GLint max_tex_size = 0;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);

  struct fileBuffer readFileBuffer;
  fread(&readFileBuffer, sizeof(fileBuffer),1,fp);

  assert(max_tex_size == readFileBuffer.max_tex_size);

  m_min.x = readFileBuffer.min_x;
  m_min.y = readFileBuffer.min_y;
  m_min.z = readFileBuffer.min_z;

  m_max.x = readFileBuffer.max_x;
  m_max.y = readFileBuffer.max_y;
  m_max.z = readFileBuffer.max_z;

  m_gridSize.x = readFileBuffer.gridSize_x;
  m_gridSize.y = readFileBuffer.gridSize_y;
  m_gridSize.z = readFileBuffer.gridSize_z;

  m_numVoxels.x = readFileBuffer.numVoxels_x;
  m_numVoxels.y = readFileBuffer.numVoxels_y;
  m_numVoxels.z = readFileBuffer.numVoxels_z;

  m_voxelSize.x = readFileBuffer.voxelSize_x;
  m_voxelSize.y = readFileBuffer.voxelSize_y;
  m_voxelSize.z = readFileBuffer.voxelSize_z;

  m_clearColor.r = readFileBuffer.clearColor_r;
  m_clearColor.g = readFileBuffer.clearColor_g;
  m_clearColor.b = readFileBuffer.clearColor_b;

  m_gridArraySize = readFileBuffer.gridArraySize;
  m_lightsArraySize = readFileBuffer.lightsArraySize;

  m_triangleVertexArraySize = readFileBuffer.triangleVertexArraySize;
  m_triangleListArraySize = readFileBuffer.triangleListArraySize;
  m_triangleNormalsArraySize = readFileBuffer.triangleNormalsArraySize;
  m_triangleMaterialArraySize = readFileBuffer.triangleMaterialArraySize;

  int texture2DnumLines = (int)((m_gridArraySize/4)/max_tex_size) + (int)(m_gridArraySize%max_tex_size != 0);
  m_gridArray = new GLfloat[texture2DnumLines*max_tex_size*4];
  fread(m_gridArray, sizeof(GLfloat),texture2DnumLines*max_tex_size*4,fp);

  m_lightsArray = new GLfloat[m_lightsArraySize];
  fread(m_lightsArray, sizeof(GLfloat),m_lightsArraySize,fp);

  texture2DnumLines = (int)((m_triangleVertexArraySize/3)/max_tex_size) + (int)(m_triangleVertexArraySize%max_tex_size != 0);
  m_triangleVertexArray = new GLfloat[texture2DnumLines*max_tex_size*3];
  fread(m_triangleVertexArray, sizeof(GLfloat),texture2DnumLines*max_tex_size*3,fp);

  texture2DnumLines = (int)((m_triangleListArraySize)/max_tex_size) + (int)(m_triangleListArraySize%max_tex_size != 0);
  m_triangleListArray = new GLfloat[texture2DnumLines*max_tex_size];
  fread(m_triangleListArray, sizeof(GLfloat),texture2DnumLines*max_tex_size,fp);

  texture2DnumLines = (int)((m_triangleNormalsArraySize/3)/max_tex_size) + (int)(m_triangleNormalsArraySize%max_tex_size != 0);
  m_triangleNormalsArray = new GLfloat[texture2DnumLines*max_tex_size*3];
  fread(m_triangleNormalsArray, sizeof(GLfloat),texture2DnumLines*max_tex_size*3,fp);

  texture2DnumLines = (int)((m_triangleMaterialArraySize/4)/max_tex_size) + (int)(m_triangleMaterialArraySize%max_tex_size != 0);
  m_triangleMaterialArray = new GLfloat[texture2DnumLines*max_tex_size*4];
  fread(m_triangleMaterialArray, sizeof(GLfloat),texture2DnumLines*max_tex_size*4,fp);

  fclose(fp);
}

Color UniformGrid::getClearColor() const
{
  return m_clearColor; 
}


