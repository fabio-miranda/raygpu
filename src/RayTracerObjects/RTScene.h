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

      bool mBinFile;
      string mRTBFileName;

      vector<RTMaterial> mMaterials;
      vector<RTLight> mLights;
      vector<RTMesh> mMeshes;
      RTCamera mCamera;

      Vector3 mGridSize;
      UniformGrid* mGrid;

      GLuint mGridTexId;
      GLuint mTrianglesTexId;
      GLuint mVertexesTexId;
      GLuint mMaterialTexId;
      //GLuint mDiffuseTexId;
      //GLuint mSpecularTexId;
      GLuint mNormalsTexId;
      
      

      GLuint mLightsTexId;;

   public:
	   RTScene(){};
      RTScene(string fileName, Vector3 gridSize = Vector3(1,1,1));
      ~RTScene();

      void configure();
      void render();

      UniformGrid* getUniformGrid() const;
      void writeRTBFile(string rtbFileName);


      GLuint getGridTexId();
      GLuint getTriangleListTexId();
      GLuint getVertexesTexId();
      GLuint getMaterialTexId();
      //GLuint getDiffuseTexId();
      //GLuint getSpecularTexId();
      GLuint getNormalsTexId();
      GLuint getLightsTexId();

      GLfloat getGridTexSize();
      GLfloat getTriangleListTexSize();
      GLfloat getVertexesTexSize();
      GLfloat getMaterialTexSize();
      //GLfloat getDiffuseTexSize();
      //GLfloat getSpecularTexSize();
      GLfloat getNormalsTexSize();
      GLfloat getLightsTexSize();

      Color getClearColor() const;

   protected:
      void readFromStr(char buffer[]);
      void readFromRT4File(string rt4FileName);
      void readFromRTBFile(string rtbFileName);
	    unsigned int getSceneNumTriangles();

      void calcTextures();
};

