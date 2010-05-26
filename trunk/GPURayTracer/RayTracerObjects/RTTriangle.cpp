/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#include <iostream>


#include "RayTracerObjects\RTTriangle.h"
#include <assert.h>

///////////////////
//~ RTTriangle
//////////////////
RTTriangle :: RTTriangle(Vector3 V1, Vector3 V2, Vector3 V3)
:v1(V1)
,v2(V2)
,v3(V3)
{
}

vector<RTTriangle> RTTriangle :: readFromFile(string fileName)
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
      tList[i].v1 = vList[t1];
      tList[i].v2 = vList[t2];
      tList[i].v3 = vList[t3];
   }
   fclose(file);

   delete[] vList;

   return tList;
}


