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
//   Color difuse, specular;
//   float alpha;
//   float shininess;
   RTTriangle() {}
   RTTriangle(Vector3 V1, Vector3 V2, Vector3 V3);

   static vector<RTTriangle> readFromFile(string fileName);
};

