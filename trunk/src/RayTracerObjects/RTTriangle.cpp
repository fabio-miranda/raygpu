/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#include <iostream>
#include <string>


#include "RayTracerObjects\RTTriangle.h"
#include <assert.h>

unsigned int RTTriangle :: sTriangleNum = 0;
using namespace std;
///////////////////
//~ RTTriangle
//////////////////
RTTriangle :: RTTriangle()
{
  mMyTriangleNum = sTriangleNum++;
}

RTTriangle :: RTTriangle(unsigned int MATERIALINDEX, Vector3 V1, Vector3 V2, Vector3 V3)
:mMaterialIndex(MATERIALINDEX)
,v1(V1)
,v2(V2)
,v3(V3)
{
  mMyTriangleNum = sTriangleNum++;
}

vector<RTTriangle> RTTriangle :: readFromFile(unsigned int materialIndex, string fileName)
{
  int index = fileName.find_last_of(".");
  assert(index!=string::npos);
  string sub = fileName.substr(index, string::npos);
  
  if(sub.compare(".um")==0 || sub.compare(".Um")==0 || sub.compare(".uM")==0 || sub.compare(".UM")==0)
    return readUmFile(materialIndex, fileName);
  else if(sub.compare(".msh")==0 || sub.compare(".MSH")==0)
    return readMshFile(materialIndex, fileName);

  vector<RTTriangle> a;
  return a;
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

vector<RTTriangle> RTTriangle::readUmFile(unsigned int materialIndex, string fileName )
{
  FILE *file;
  file = fopen(fileName.c_str(), "rt");
  assert(file);

  int numVertex, numRTTriangles;

  fscanf(file, "%d\n", &numVertex);
  printf("Reading %d Vertices...\n", numVertex);

  Vector3 * vList = new Vector3[numVertex];

  for(int i = 0; i < numVertex; ++i)
  {
    Vector3 a;
    //fscanf(file, "%f %f %f\n", &vList[i].z, &vList[i].y, &vList[i].x);
    fscanf(file, "%f %f %f\n", &vList[i].x, &vList[i].y, &vList[i].z); 
    //fscanf(file, "%f %f %f\n", &(a.y), &(a.x), &(a.z)); 
    //vList[i].y = a.y;
    //vList[i].x = a.x;
    //vList[i].z = a.z;

    //fscanf(file, "%f %f %f\n", &(vList[i].y), &(vList[i].x), &(vList[i].z)); 
  }

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

vector<RTTriangle> RTTriangle::readMshFile(unsigned int materialIndex, string fileName )
{
  FILE *file;
  file = fopen(fileName.c_str(), "rt");
  assert(file);

  int numVertex, numRTTriangles;

  fscanf(file, "%d %d\n", &numVertex, &numRTTriangles);
  printf("Reading %d Vertices...\n", numVertex);

  Vector3 * vList = new Vector3[numVertex];

  for(int i = 0; i < numVertex; ++i)
  {
    Vector3 a;
    //fscanf(file, "%f %f %f\n", &vList[i].z, &vList[i].y, &vList[i].x);
    fscanf(file, "%*d %f %f %f\n", &vList[i].x, &vList[i].y, &vList[i].z); 
    //fscanf(file, "%f %f %f\n", &(a.y), &(a.x), &(a.z)); 
    //vList[i].y = a.y;
    //vList[i].x = a.x;
    //vList[i].z = a.z;

    //fscanf(file, "%f %f %f\n", &(vList[i].y), &(vList[i].x), &(vList[i].z)); 
  }

  printf("Reading %d RTTriangles...\n", numRTTriangles);

  vector<RTTriangle> tList = vector<RTTriangle>();
  tList.reserve(numRTTriangles);
  for(int i = 0; i < numRTTriangles; ++i)
  {
    int t1, t2, t3;
    fscanf(file, "%*d %d %d %d\n", &t1, &t2, &t3);
    RTTriangle rt(materialIndex, vList[t1], vList[t2], vList[t3]);
    tList.push_back(rt);
  }

  fclose(file);

  delete[] vList;

  return tList;
}


