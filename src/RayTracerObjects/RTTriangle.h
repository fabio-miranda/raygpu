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
public:
   Vector3 v1, v2, v3;
   unsigned int index;
   unsigned int materialIndex;
//   Color difuse, specular;
//   float alpha;
//   float shininess;
   RTTriangle() {}
   RTTriangle(unsigned int index, unsigned int materialIndex, Vector3 V1, Vector3 V2, Vector3 V3);

   static vector<RTTriangle> readFromFile(unsigned int materialIndex, string fileName);
};

