/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#include <iostream>


#include "RayTracerObjects\RTTriangle.h"
#include <assert.h>

unsigned int RTTriangle :: sTriangleMax = 0;
unsigned int RTTriangle :: sTriangleNum = 0;

///////////////////
//~ RTTriangle
//////////////////
RTTriangle :: RTTriangle()
{
  sTriangleNum++;
  sTriangleMax = sTriangleNum;
}

RTTriangle :: RTTriangle(unsigned int MATERIALINDEX, Vector3 V1, Vector3 V2, Vector3 V3)
:mMaterialIndex(MATERIALINDEX)
,v1(V1)
,v2(V2)
,v3(V3)
{
  sTriangleNum++;
  sTriangleMax = sTriangleNum;
}

vector<RTTriangle> RTTriangle :: readFromFile(unsigned int materialIndex, string fileName)
{
   FILE *file;
   file = fopen(fileName.c_str(), "rt");
   assert(file);

   int numVertex, numRTTriangles;

   fscanf(file, "%d\n", &numVertex);
   printf("Reading %d Vertices...\n", numVertex);

   Vector3 * vList = new Vector3[numVertex];

   for(int i = 0; i < numVertex; ++i)
      fscanf(file, "%f %f %f\n", &vList[i].x, &vList[i].y, &vList[i].z);

   fscanf(file, "\n%d\n", &numRTTriangles);
   printf("Reading %d RTTriangles...\n", numRTTriangles);

   vector<RTTriangle> tList = vector<RTTriangle>(numRTTriangles);
   for(int i = 0; i < numRTTriangles; ++i)
   {
      int t1, t2, t3;
      fscanf(file, "%d %d %d\n", &t1, &t2, &t3);
	    tList[i].mMaterialIndex = materialIndex;
      tList[i].v1 = vList[t1];
      tList[i].v2 = vList[t2];
      tList[i].v3 = vList[t3];
   }
   sTriangleMax += numRTTriangles - 1; //O construtor do vector só chama 
                                      //o construtor do RTTRianglus uma vez e é preciso 
                                     //incrementar o contador de triangulos
   fclose(file);

   delete[] vList;

   return tList;
}

unsigned int RTTriangle::getGlobalIndex()
{
  return sTriangleNum;
}

unsigned int RTTriangle::getMaxNumTriangles()
{
  return sTriangleMax;
}

unsigned int RTTriangle::getMaterialIndex()
{
  return mMaterialIndex;
}


