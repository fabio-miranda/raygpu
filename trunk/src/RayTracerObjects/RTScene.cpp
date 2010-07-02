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
  GLuint id[] = {
    mGridTexId,
    mVertexesTexId,
    mAmbientTexId,
    mDiffuseTexId,
    mSpecularTexId,
    mNormalsTexId,
    mTrianglesTexId,
    mLightsTexId
  };
  glDeleteTextures(sizeof(id)/sizeof(GLuint), id);
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
    mGrid = new UniformGrid(getSceneNumTriangles(), &mMeshes, &mMaterials, Vector3(10,10,10));
    //mGrid = new UniformGrid(getSceneNumTriangles(), &mMeshes, &mMaterials, Vector3(2,2,2));
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


  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  vector<RTLight> :: iterator lightIt;
  for( lightIt = mLights.begin(); lightIt!=mLights.end(); ++lightIt)
    lightIt->render();

  vector<RTMesh> :: iterator meshIt;
  for( meshIt = mMeshes.begin(); meshIt!=mMeshes.end(); ++meshIt)
  {
    glPushAttrib(GL_LIGHTING_BIT);
    mMaterials[meshIt->getMaterialIndex()].render();
    //meshIt->render();
    glPopAttrib();
  }
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glCullFace(GL_BACK);
  glPopAttrib();

   //mGrid->render();
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
  GLint max_tex_size = 0;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);

  GLenum sizeType [] = {0,GL_ALPHA, 2 ,GL_RGB, GL_RGBA};
  
  GLfloat* lData = new GLfloat[mLights.size()*sizeof(struct lightStruct)];
  for( unsigned int i = 0; i < mLights.size(); ++i)
    memcpy(&lData[i*sizeof(struct lightStruct)/sizeof(GLfloat)], mLights[i].getLightStruct(), sizeof(struct lightStruct));
  int lightArraySize = mLights.size()*sizeof(struct lightStruct)/(4*sizeof(GLfloat));
  int lightArrayAbsoluteSize = mLights.size()*sizeof(struct lightStruct)/(sizeof(GLfloat));


  GLfloat* data[] = {   mGrid->getGridArray()/*RGBA*/,
                        lData/*RGBA*/};
  
  
  unsigned int size[] = {   mGrid->getGridArraySize(),
                            lightArraySize };
  
  int sizeIndex [] = {  mGrid->getGridArrayAbsoluteSize()/mGrid->getGridArraySize(),
                        lightArrayAbsoluteSize/lightArraySize
                        };
  
  int numTextures = sizeof(data)/sizeof(GLfloat*);
  GLuint *id = new GLuint[numTextures];
  //// //DEBUG
  //char sizeTypeStr [5][10] = {"NOT","GL_ALPHA", "2" ,"GL_RGB", "GL_RGBA"};
  //for(int i=0;i<numTextures;++i)
  //{
  //  printf("%d %s\n",sizeIndex[i], sizeTypeStr[sizeIndex[i]]);
  //}


  glGenTextures(numTextures, id);
  for(int i=0; i<numTextures; ++i)
  {
    if(size[i]<max_tex_size)
    {
      glBindTexture(GL_TEXTURE_1D, id[i]);
      //   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
      glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F_ARB, size[i], 0, sizeType[sizeIndex[i]], GL_FLOAT, data[i]);
      glBindTexture(GL_TEXTURE_1D, 0);

    }else 
    {
      printf("Texture 1D too big!");
      assert(false);
    }
  }

  /////2D
  /////2D
  /////2D
  /////2D

  GLfloat* data2D[] = {  mGrid->getTriangleVertexArray()/*RGB*/,
							           mGrid->getTriangleListArray()/*A*/,
                         mGrid->getTriangleAmbientArray()/*RGB*/,
                         mGrid->getTriangleDiffuseArray()/*RGB*/,
                         mGrid->getTriangleSpecularArray()/*RGBA*/,
                         mGrid->getTriangleNormalsArray()/*RGB*/
                        };

  unsigned int size2D[] = { mGrid->getTriangleVertexArraySize(),
							              mGrid->getTriangleListArraySize(),
                            mGrid->getTriangleAmbientArraySize(),
                            mGrid->getTriangleDiffuseArraySize(), 
                            mGrid->getTriangleSpecularArraySize(),
                            mGrid->getTriangleNormalsArraySize()
                          };

  int sizeIndex2D[] = { mGrid->getTriangleVertexArrayAbsoluteSize()/mGrid->getTriangleVertexArraySize(),
						            mGrid->getTriangleListArrayAbsoluteSize()/mGrid->getTriangleListArraySize(),
                        mGrid->getTriangleAmbientArrayAbsoluteSize()/mGrid->getTriangleAmbientArraySize(),
                        mGrid->getTriangleDiffuseArrayAbsoluteSize()/mGrid->getTriangleDiffuseArraySize(), 
                        mGrid->getTriangleSpecularArrayAbsoluteSize()/mGrid->getTriangleSpecularArraySize(), 
                        mGrid->getTriangleNormalsArrayAbsoluteSize()/mGrid->getTriangleNormalsArraySize()
                    };

  int numTextures2D = sizeof(data2D)/sizeof(GLfloat*);
  GLuint *id2D = new GLuint[numTextures2D];

  glGenTextures(numTextures2D, id2D);
  for(int i=0; i<numTextures2D; ++i)
  {
    if (size2D[i]<max_tex_size*max_tex_size)
    {
      glBindTexture(GL_TEXTURE_2D, id2D[i]);
      //   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, max_tex_size, (size2D[i]/max_tex_size)+1, 0, sizeType[sizeIndex2D[i]], GL_FLOAT, data2D[i]);
      glBindTexture(GL_TEXTURE_2D, 0);
    }else 
    {
      printf("Texture 2D is too big!");
      assert(false);
    }
  }

  
  int i = 0;
  mGridTexId = id[i++];
  mLightsTexId = id[i++];
  
  i = 0;
  mVertexesTexId = id2D[i++];
  mTrianglesTexId = id2D[i++];
  mAmbientTexId = id2D[i++];
  mDiffuseTexId = id2D[i++];
  mSpecularTexId = id2D[i++];
  mNormalsTexId = id2D[i++];
  
  delete[] id;
  delete[] id2D;
	/*
	int texture2DnumLines = (int)((mGrid->getTriangleListArraySize())/max_tex_size) + (int)(mGrid->getTriangleListArraySize()%max_tex_size != 0);
	for(int i = 0; i < texture2DnumLines*max_tex_size; ++i)
		std::cout << mGrid->getTriangleListArray()[i] << " ";
	*/
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
  return  mLights.size()*sizeof(struct lightStruct)/(4*sizeof(GLfloat));
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



