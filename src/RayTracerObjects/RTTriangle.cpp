/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#include <iostream>


#include "RayTracerObjects\RTTriangle.h"
#include <assert.h>

unsigned int RTTriangle :: sTriangleNum = 0;

///////////////////
//~ RTTriangle
//////////////////
RTTriangle :: RTTriangle()
{
  sTriangleNum++;
  mMyTriangleNum = sTriangleNum;
}

RTTriangle :: RTTriangle(unsigned int MATERIALINDEX, Vector3 V1, Vector3 V2, Vector3 V3)
:mMaterialIndex(MATERIALINDEX)
,v1(V1)
,v2(V2)
,v3(V3)
{
  sTriangleNum++;
  mMyTriangleNum = sTriangleNum;
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

   vector<RTTriangle> tList = vector<RTTriangle>();
   tList.reserve(numRTTriangles);
   for(int i = 0; i < numRTTriangles; ++i)
   {
      int t1, t2, t3;
      fscanf(file, "%d %d %d\n", &t1, &t2, &t3);
	    RTTriangle rt(materialIndex, vList[t1], vList[t2], vList[t3]);
      tList.push_back(rt);
   }

   fclose(file);

   delete[] vList;

   return tList;
}

unsigned int RTTriangle::getGlobalIndex()
{
  return mMyTriangleNum;
}

unsigned int RTTriangle::getMaxNumTriangles()
{
  return sTriangleNum;
}

unsigned int RTTriangle::getMaterialIndex()
{
  return mMaterialIndex;
}


