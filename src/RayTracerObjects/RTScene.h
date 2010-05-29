/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once

#include <vector>
#include <string>
#include "string.h"

#include "GraphBasis/Vector3.h"
#include "GraphBasis/Color.h"

#include "RayTracerObjects\RTCamera.h"
#include "RayTracerObjects\RTLight.h"
#include "RayTracerObjects\RTMesh.h"
#include "RayTracerObjects\RTMaterial.h"
#include "RayTracerObjects\RTTriangle.h"

#include "SpacePartition/UniformGrid.h"

using namespace std;

class RTScene
{
   protected:
      Color mClear;
      Color mAmbient;

      vector<RTMaterial> mMaterials;
      vector<RTLight> mLights;
      vector<RTMesh> mMeshes;
      RTCamera mCamera;

   public:
      RTScene(string rt4FileName);
      void readFromStr(char buffer[]);

      void configure();
      void render();

   protected:
      void readFromFile(string rt4FileName);
	  unsigned int getSceneNumTriangles();

};

