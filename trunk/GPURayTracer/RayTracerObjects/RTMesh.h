/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once
#include <string>

#include "GraphBasis/Vector3.h"
#include "RayTracerObjects/RTTriangle.h"

using namespace std;

class VertexBufferObject;

class RTMesh
{
   static int sMeshNum;

   protected:
      VertexBufferObject* mVbo;
      bool mCalculed;

      int mMaterialIndex;
      Vector3 mPos;
      Vector3 mPos2;
      vector<RTTriangle> mTriangles;

      int myRTMeshNum;
   public:
      RTMesh();

      int getMyRTMeshNumber() const;
      void readFromStr(char buffer[]);

      int getMaterialIndex();

      void configure();
      void render();
   protected:
       void calcVBO();
};

