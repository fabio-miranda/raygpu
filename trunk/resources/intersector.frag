uniform sampler2D rayPos;
uniform sampler2D rayDir;

uniform sampler1D grid;
uniform sampler1D triangleList;
uniform sampler1D vertexes;

uniform float gridSize;
uniform float triangleListSize;
uniform float vertexesSize;

#define ContainTriangles 1
#define Empty 0

//enum {Traversing = 0, Intersecting , Shading, Done}

const float infinity = 1.0/0.0;
vec3 intersect(float vertexIndex, vec4 rPos, vec4 rDir, vec3 lastHit, float triangleIndex);

void main()
{
   vec4 rDir = texture2D(rayDir, gl_TexCoord[0].st);
   vec4 rPos = texture2D(rayPos, gl_TexCoord[0].st);

   int triangleFlag = int(floor(rDir.w+.5));
   float gridIndex =  floor(rPos.w+.5);

   if(triangleFlag == ContainTriangles)
   {

      float triangleIndex = floor(texture1D(grid, gridIndex/gridSize).r + .5);
      float vertexIndex = floor(texture1D(triangleList, triangleIndex/triangleListSize).r + .5);
      vec3 lastHit = vec3(infinity, vertexIndex, triangleIndex);

      while(vertexIndex != -1.0)
      {
         lastHit = intersect(vertexIndex, rPos, rDir, lastHit, triangleIndex);
         triangleIndex++;
         vertexIndex = floor(texture1D(triangleList, triangleIndex/triangleListSize).r + .5);
      }

      if(lastHit.r != infinity)
      {
         ///Set Stencil Bufer to Shading
         vec3 fragPos = rPos.xyz+rDir.xyz*lastHit.r;
         vec4 triangleInfo = vec4(fragPos, lastHit.b);
         gl_FragData[0] = triangleInfo;
         return;
      }
   }

   ///Set Stencil Bufer to Traversing
//   gl_FragData[0] = rDir;
//   gl_FragData[1] = rPos;
}


vec3 intersect(float vertexIndex, vec4 rPos, vec4 rDir, vec3 lastHit, float triangleIndex)
{
   vec3 v0 = texture1D(vertexes, (vertexIndex*3.)/vertexesSize).xyz;
   vec3 v1 = texture1D(vertexes, (vertexIndex*3.+1.)/vertexesSize).xyz;
   vec3 v2 = texture1D(vertexes, (vertexIndex*3.+2.)/vertexesSize).xyz;

   vec3 edge1 = v1 - v0;
   vec3 edge2 = v2 - v0;

   vec3 pvec, qvec;
   float det, inv_det;
   float u,v,t;

   //find vectors for two edges sharing v0
  //begin calculating determinant – also used to calculate U param
   pvec = cross(rDir.xyz, edge2);

   //if determinant is near zero, ray lies in plane of triangle
   det = dot(pvec, edge1);

   if(det < 0.000001)
      return lastHit ;

   //calculate distance from v0 to ray origin
   vec3 tvec = rPos.xyz;
   tvec = tvec - v0;
//   tvec = rPos.xyz – v0;

   //calculate U param and test bounds
   u = dot(tvec, pvec);
   if(u < 0.0 || u > det)
      return lastHit;

   //prepare to test V param
   qvec = cross(tvec, edge1);

   //calculate V param and test bounds
   v = dot(rDir.xyz, qvec);
   if(v < 0.0 || u + v > det)
      return lastHit;

   //calculate t, scale params, ray intersect triangle
   t = dot(edge2, qvec);
   inv_det = 1.0 / det;
   t*=inv_det;
   //u*=inv_det;
   //v*=inv_det;

   if(t < lastHit.r)
      return vec3(t, vertexIndex, triangleIndex);
   else
      return lastHit;
}

