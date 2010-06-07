/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "RayTracerObjects/RTScene.h"

using namespace std;

///////////////////
//~ RTScene
//////////////////
RTScene :: RTScene(string rt4FileName)
:mCalculed(false)
,mGrid(NULL)
{
   readFromFile(rt4FileName);
}

RTScene :: ~RTScene()
{
  delete mGrid;
  GLuint id[7] = {
    mGridTexId,
    mVertexesTexId,
    mAmbientTexId,
    mDiffuseTexId,
    mSpecularTexId,
    mNormalsTexId,
    mTrianglesTexId
  };
  glDeleteTextures(7, id);
  glDeleteTextures(1, &mLightsTexId);
}

void RTScene :: readFromFile(string rt4FileName)
{
	FILE *file;
	char buffer[1024];

	file = fopen(rt4FileName.c_str(), "rt");
	assert(file);

	int numScene = 0;
	int numCamera = 0;
	int numMaterials = 0;
	int numMeshes = 0;
	int numLights = 0;

	while(!feof(file))
	{
	   fscanf(file, "%s", buffer);
//	   cout << buffer <<endl;
	   if(!strcmp(buffer, "RT"))
	   {
			/* Ignore File Version Information */
         fscanf(file, "%*[^\n]s");
	   }else if(!strcmp(buffer, "CAMERA"))
      {
         numCamera++;
         if(numCamera > 1)
         {
            fscanf(file, "%*[^\n]s");
            cout << "Cameras Multiplas definidas!" <<endl;
         }else
         {
            fscanf(file, "%[^\n]s", buffer);
            mCamera.readFromStr(buffer);
         }
      }else if(!strcmp(buffer, "SCENE"))
      {
         numScene++;
         if(numScene > 1)
         {
            fscanf(file, "%*[^\n]s");
            cout << "Cenas Multiplas definidas!" <<endl;
         }else
         {
            fscanf(file, "%[^\n]s", buffer);
            readFromStr(buffer);
         }
		}else if(!strcmp(buffer, "MATERIAL"))
		{
         numMaterials++;
         fscanf(file, "%[^\n]s", buffer);
         RTMaterial m;
         m.readFromStr(buffer);
         mMaterials.push_back(m);
		}else if(!strcmp(buffer, "LIGHT"))
		{
         numLights++;
         if(numScene > RTLight::Max_Lights)
         {
            fscanf(file, "%*[^\n]s");
            cout << "Maximo de Luzes definidas!" <<endl;
         }else
         {
            fscanf(file, "%[^\n]s", buffer);
            RTLight l;
            l.readFromStr(buffer);
            mLights.push_back(l);
         }
      }else if(!strcmp(buffer, "MESH"))
		{
         numMeshes++;
         fscanf(file, "%[^\n]s", buffer);
         RTMesh m;
         m.readFromStr(buffer);
         mMeshes.push_back(m);
		}else
		{
			printf( "Ignorando comando: %s\n", buffer );
         fscanf(file, "%*[^\n]s");
		}
	}
   fclose( file );

	assert(numCamera == 1);
	assert(numScene == 1);
}

void RTScene :: readFromStr(char buffer[])
{
   int r = sscanf( buffer, "%f %f %f %f %f %f %*s\n", &mClear.r, &mClear.g, &mClear.b,
      &mAmbient.r, &mAmbient.g, &mAmbient.b); //,* == backgroundFileName
   assert(r == 7-1);//-1 == backgroundFileName

//   cout <<"Scene Clear, Ambient" << mClear  << mAmbient<<endl;
}

void RTScene :: configure()
{
  if(!mCalculed) 
  {
    if(mGrid)
      delete mGrid;
    mGrid = new UniformGrid(getSceneNumTriangles(), &mMeshes, &mMaterials, Vector3(10, 10, 10));
    calcTextures();
    mCalculed = true;
  }
  
  vector<RTLight> :: iterator lightIt;
   for( lightIt = mLights.begin(); lightIt!=mLights.end(); ++lightIt)
      lightIt->configure();

   vector<RTMesh> :: iterator meshIt;
   for( meshIt = mMeshes.begin(); meshIt!=mMeshes.end(); ++meshIt)
   {
      mMaterials[meshIt->getMaterialIndex()].configure();
      meshIt->configure();
   }
}


void RTScene :: render()
{
   glPushAttrib(GL_LIGHTING_BIT);
      vector<RTLight> :: iterator lightIt;
      for( lightIt = mLights.begin(); lightIt!=mLights.end(); ++lightIt)
         lightIt->render();

      vector<RTMesh> :: iterator meshIt;
      for( meshIt = mMeshes.begin(); meshIt!=mMeshes.end(); ++meshIt)
      {
         glPushAttrib(GL_LIGHTING_BIT);
            mMaterials[meshIt->getMaterialIndex()].render();
            meshIt->render();
         glPopAttrib();
      }
   glPopAttrib();
}

UniformGrid* RTScene ::GetUniformGrid() const 
{ 
  return mGrid; 
}
unsigned int RTScene::getSceneNumTriangles()
{
  return RTTriangle::getMaxNumTriangles();
}




void RTScene::calcTextures()
{
  GLuint id[7];
  

  GLfloat* data[] = { mGrid->getGridArray()/*A*/,
                       mGrid->getTriangleVertexArray()/*RGB*/, mGrid->getTriangleAmbientArray()/*RGB*/,
                       mGrid->getTriangleDiffuseArray()/*RGB*/, mGrid->getTriangleSpecularArray()/*RGBA*/,
                        mGrid->getTriangleListArray()/*A*/, mGrid->getTriangleNormalsArray()/*RGB*/};
  
  unsigned int size[] = {  mGrid->getGridArraySize(),
                            mGrid->getTriangleVertexArraySize(), mGrid->getTriangleAmbientArraySize(),
                            mGrid->getTriangleDiffuseArraySize(), mGrid->getTriangleSpecularArraySize(),
                            mGrid->getTriangleListArraySize(), mGrid->getTriangleNormalsArraySize()};
  glGenTextures(7, id);
  for(int i=0; i<7; ++i)
  {
    glBindTexture(GL_TEXTURE_1D, id[i]);
    //   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F_ARB, size[i], 0, GL_RGBA, GL_FLOAT, data[i]);
    glBindTexture(GL_TEXTURE_1D, 0);
  }
  glGenTextures(1, &mLightsTexId);
  GLfloat* lData = new GLfloat[mLights.size()*sizeof(struct lightStruct)];
  for( unsigned int i = 0; i < mLights.size(); ++i)
    memcpy(&lData[i*sizeof(struct lightStruct)], mLights[i].getLightStruct(), sizeof(struct lightStruct));
 
  glBindTexture(GL_TEXTURE_1D, mLightsTexId);
  //   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F_ARB, mLights.size()*sizeof(struct lightStruct)/sizeof(GLfloat), 0, GL_RGBA, GL_FLOAT,  lData);
  glBindTexture(GL_TEXTURE_1D, 0);


  mGridTexId = id[0];
  mVertexesTexId = id[1];
  mAmbientTexId = id[2];
  mDiffuseTexId = id[3];
  mSpecularTexId = id[4];
  mTrianglesTexId = id[5];
  mNormalsTexId = id[6];
}

GLuint RTScene::getGridTexId()
{
  return mGridTexId;
  
}

GLuint RTScene::getTriangleListTexId()
{
  return mTrianglesTexId;
}

GLuint RTScene::getVertexesTexId()
{
  return mVertexesTexId;
}

GLuint RTScene::getAmbientTexId()
{
  return mAmbientTexId;
}

GLuint RTScene::getDiffuseTexId()
{
  return mDiffuseTexId;
}

GLuint RTScene::getSpecularTexId()
{
  return mSpecularTexId;
}

GLuint RTScene::getNormalsTexId()
{
  return mNormalsTexId;
}

GLuint RTScene::getLightsTexId()
{
  return mLightsTexId;
}


GLfloat RTScene::getLightsTexSize()
{
  return  mLights.size()*sizeof(struct lightStruct)/sizeof(GLfloat);
}

GLfloat RTScene::getGridTexSize()
{
  return mGrid->getGridArraySize();
}

GLfloat RTScene::getTriangleListTexSize()
{
  return mGrid->getTriangleListArraySize();
}

GLfloat RTScene::getVertexesTexSize()
{
  return mGrid->getTriangleVertexArraySize();
}

GLfloat RTScene::getAmbientTexSize()
{
  return  mGrid->getTriangleAmbientArraySize();
}

GLfloat RTScene::getDiffuseTexSize()
{
  return mGrid->getTriangleDiffuseArraySize();
}

GLfloat RTScene::getSpecularTexSize()
{
return mGrid->getTriangleSpecularArraySize();
}

GLfloat RTScene::getNormalsTexSize()
{
  return mGrid->getTriangleNormalsArraySize();
}



