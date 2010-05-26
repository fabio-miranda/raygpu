/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#include <iostream>

#include "RayTracerObjects/RTMesh.h"

using namespace std;

int RTMesh :: sMeshNum = 0;

///////////////////
//~ RTMesh
//////////////////
RTMesh :: RTMesh()
{
   myRTMeshNum = sMeshNum++;
}


int RTMesh :: getMyRTMeshNumber() const
{
   return myRTMeshNum;
}

void RTMesh :: readFromStr(char buffer[])
{
   int r = sscanf( buffer, "%d %f %f %f %f %f %f %s\n", &mMaterialIndex, &mPos.x, &mPos.y, &mPos.z,
      &mPos2.x, &mPos2.y, &mPos2.z, buffer);
   cout << "Ler Arquivo " << buffer <<endl;
   mTriangles = RTTriangle::readFromFile(string(buffer));

   assert(r == 8);

//   cout << "Mesh materialindex, pos, pos2, fileName:\n" << mMaterialIndex <<endl << mPos << mPos2 << buffer <<endl;
}




