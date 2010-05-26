/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once
#include <string>

#include "GraphBasis/Vector3.h"
#include "RayTracerObjects/RTTriangle.h"

using namespace std;

class RTMesh
{
   static int sMeshNum;

   protected:
      int mMaterialIndex;
      Vector3 mPos;
      Vector3 mPos2;
      vector<RTTriangle> mTriangles;

      int myRTMeshNum;
   public:
      RTMesh();

      int getMyRTMeshNumber() const;
      void readFromStr(char buffer[]);

};

