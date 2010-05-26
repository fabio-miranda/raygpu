/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once

#include <vector>
#include <string>

#include "GraphBasis/Vector3.h"
#include "GraphBasis/Color.h"

#include "RayTracerObjects\RTCamera.h"
#include "RayTracerObjects\RTLight.h"
#include "RayTracerObjects\RTMesh.h"
#include "RayTracerObjects\RTMaterial.h"
#include "RayTracerObjects\RTTriangle.h"

using namespace std;

class RTScene
{
   protected:
      Color mClear;
      Color mAmbient;

      vector<RTTriangle> mTriangles;
      vector<RTMaterial> mMaterials;
      vector<RTLight> mLights;
      vector<RTMesh> mMeshes;
      RTCamera mCamera;

   public:
      RTScene(string rt4FileName, string umFileName);
      void readFromStr(char buffer[]);

   protected:
      void readFromFile(string rt4FileName, string umFileName);

};

