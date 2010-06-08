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
      bool mCalculed;
      Color mClear;
      Color mAmbient;

      vector<RTMaterial> mMaterials;
      vector<RTLight> mLights;
      vector<RTMesh> mMeshes;
      RTCamera mCamera;

      UniformGrid* mGrid;

      GLuint mGridTexId;
      GLuint mTrianglesTexId;
      GLuint mVertexesTexId;
      GLuint mAmbientTexId;
      GLuint mDiffuseTexId;
      GLuint mSpecularTexId;
      GLuint mNormalsTexId;
      
      

      GLuint mLightsTexId;;

   public:
	   RTScene(){};
      RTScene(string rt4FileName);
      ~RTScene();

      void configure();
      void render();

      UniformGrid* GetUniformGrid() const;


      GLuint getGridTexId();
      GLuint getTriangleListTexId();
      GLuint getVertexesTexId();
      GLuint getAmbientTexId();
      GLuint getDiffuseTexId();
      GLuint getSpecularTexId();
      GLuint getNormalsTexId();
      GLuint getLightsTexId();

      GLfloat getGridTexSize();
      GLfloat getTriangleListTexSize();
      GLfloat getVertexesTexSize();
      GLfloat getAmbientTexSize();
      GLfloat getDiffuseTexSize();
      GLfloat getSpecularTexSize();
      GLfloat getNormalsTexSize();
      GLfloat getLightsTexSize();

   protected:
      void readFromStr(char buffer[]);
      void readFromFile(string rt4FileName);
	    unsigned int getSceneNumTriangles();

      void calcTextures();
};

