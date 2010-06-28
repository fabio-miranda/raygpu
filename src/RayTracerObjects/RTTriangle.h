/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once

#include <vector>
#include "GraphBasis/Vector3.h"
#include "GraphBasis/Color.h"

using namespace std;

class RTTriangle
{
    static unsigned int sTriangleNum;

    unsigned int mMaterialIndex;
    int mMyTriangleNum;
public:
   Vector3 v1, v2, v3;
   RTTriangle();
   RTTriangle(unsigned int materialIndex, Vector3 V1, Vector3 V2, Vector3 V3);

   unsigned int getMaterialIndex();
   unsigned int getGlobalIndex();
   static unsigned int getMaxNumTriangles();

   static vector<RTTriangle> readFromFile(unsigned int materialIndex, string fileName);
protected:
    static vector<RTTriangle> readUmFile(unsigned int materialIndex, string fileName);
    static vector<RTTriangle> readMshFile(unsigned int materialIndex, string fileName);

};

