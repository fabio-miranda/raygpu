/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#include <iostream>

#include "RayTracerObjects/RTScene.h"

using namespace std;

///////////////////
//~ RTScene
//////////////////
RTScene :: RTScene(string rt4FileName)
{
   readFromFile(rt4FileName);

   
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
   vector<RTLight> :: iterator lightIt;
   for( lightIt = mLights.begin(); lightIt!=mLights.end(); ++lightIt)
      lightIt->configure();

   vector<RTMesh> :: iterator meshIt;
   for( meshIt = mMeshes.begin(); meshIt!=mMeshes.end(); ++meshIt)
   {
      mMaterials[meshIt->getMaterialIndex()].configure();
      meshIt->configure();
   }

   UniformGrid* grid = new UniformGrid(getSceneNumTriangles(), &mMeshes, &mMaterials, Vector3(10, 10, 10));


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

unsigned int RTScene::getSceneNumTriangles(){

	unsigned int numTriangles = 0;
	std::vector<RTMesh> :: iterator meshIt;
	for(meshIt = mMeshes.begin(); meshIt!=mMeshes.end(); ++meshIt){
		std::vector<RTTriangle>* trianglesArray = meshIt->getTriangles();
		numTriangles+=trianglesArray->size();
	}

	return numTriangles;

}
