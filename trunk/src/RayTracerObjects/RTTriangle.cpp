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

RTTriangle :: RTTriangle(unsigned int materialIndex, Vector3 V1, Vector3 V2, Vector3 V3, Vector3 N1, Vector3 N2, Vector3 N3)
:mMaterialIndex(materialIndex)
,v1(V1)
,v2(V2)
,v3(V3)
,n1(N1)
,n2(N2)
,n3(N3)
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
  
  cout << "Unknown File Type:" << fileName << endl;
  assert(false);

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
  Vector3 * nList = new Vector3[numVertex];
  
  for(int i = 0; i < numVertex; ++i)
  {
    nList[i] = Vector3(0,0,0);
    //fscanf(file, "%f %f %f\n", &vList[i].z, &vList[i].y, &vList[i].x);
    fscanf(file, "%f %f %f\n", &vList[i].x, &vList[i].y, &vList[i].z); 

    //fscanf(file, "%f %f %f\n", &(vList[i].y), &(vList[i].x), &(vList[i].z)); 
  }

  fscanf(file, "\n%d\n", &numRTTriangles);
  printf("Reading %d RTTriangles...\n", numRTTriangles);

  Vector3 * tList = new Vector3[numRTTriangles];

  for(int i = 0; i < numRTTriangles; ++i)
  {
    int t1, t2, t3;
    fscanf(file, "%d %d %d\n", &t1, &t2, &t3);

    nList[t1] += (vList[t2] - vList[t1]) ^ (vList[t3] - vList[t2]);
    nList[t2] += (vList[t2] - vList[t1]) ^ (vList[t3] - vList[t2]);
    nList[t3] += (vList[t2] - vList[t1]) ^ (vList[t3] - vList[t2]);

    tList[i] = Vector3(t1,t2,t3);
  }

  for(int i = 0; i < numVertex; ++i)
  {
    nList[i] = nList[i].unitary();
  }

  vector<RTTriangle> tVector = vector<RTTriangle>();
  tVector.reserve(numRTTriangles);
  for(int i = 0; i < numRTTriangles; ++i)
  {
    Vector3 t = tList[i];
    int t1 = t.x, t2 = t.y, t3 = t.z;

    RTTriangle rt(materialIndex, vList[t1], vList[t2], vList[t3], nList[t1], nList[t2], nList[t3] );
    tVector.push_back(rt);
  }

  fclose(file);

  delete[] vList;
  delete[] nList;
  delete[] tList;

  return tVector;
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
  Vector3 * nList = new Vector3[numVertex];
  Vector3 * tList = new Vector3[numRTTriangles];

  for(int i = 0; i < numVertex; ++i)
  {
    nList[i] = Vector3(0,0,0);

    //fscanf(file, "%f %f %f\n", &vList[i].z, &vList[i].y, &vList[i].x);
    fscanf(file, "%*d %f %f %f\n", &vList[i].x, &vList[i].y, &vList[i].z); 

    //fscanf(file, "%f %f %f\n", &(vList[i].y), &(vList[i].x), &(vList[i].z)); 
    vList[i]*=200;
  }

  printf("Reading %d RTTriangles...\n", numRTTriangles);

   for(int i = 0; i < numRTTriangles; ++i)
  {
    int t1, t2, t3;
    fscanf(file, "%*d %d %d %d\n", &t1, &t2, &t3);
    
    nList[t1] += (vList[t2] - vList[t1]) ^ (vList[t3] - vList[t2]);
    nList[t2] += (vList[t2] - vList[t1]) ^ (vList[t3] - vList[t2]);
    nList[t3] += (vList[t2] - vList[t1]) ^ (vList[t3] - vList[t2]);

    tList[i] = Vector3(t1,t2,t3);
  }

  for(int i = 0; i < numVertex; ++i)
  {
    nList[i] = nList[i].unitary();
  }

  vector<RTTriangle> tVector = vector<RTTriangle>();
  tVector.reserve(numRTTriangles);
  for(int i = 0; i < numRTTriangles; ++i)
  {
    Vector3 t =  tList[i];
    int t1 = t.x, t2 = t.y, t3 = t.z;
    
    RTTriangle rt(materialIndex, vList[t1], vList[t2], vList[t3], nList[t1], nList[t2], nList[t3] );
    tVector.push_back(rt);
  }

  fclose(file);

  delete[] vList;
  delete[] nList;
  delete[] tList;

  return tVector;
}


