uniform sampler2D rayPos;
uniform sampler2D rayDir;
uniform sampler2D triangleInfoTex;
uniform sampler2D shadowInfoTex;

uniform sampler2D grid;
uniform sampler2D vertexes;
uniform sampler2D triangleList;





uniform float maxTextureSize;

uniform float gridSize;
uniform float triangleListSize;
uniform float vertexesSize;

#define INACTIVE 0.0
#define ACTIVE_CALCULATEOUTVOXEL 1.0
#define ACTIVE_TRAVERSE 2.0
#define ACTIVE_INTERSECT 3.0
#define ACTIVE_TRAVERSE_SEC 4.0
#define ACTIVE_SHADING 5.0
#define OVERFLOW 6.0
#define DONE 7.0


#define SHADOW_INACTIVE -1.0
#define SHADOW_ACTIVE 1.0

vec2 index1Dto2D(float index, float width, float size)
{
  float height = float(trunc(size/width))+1.0;
  vec2 r = vec2(float(mod(index,width)), float(trunc(index/width)));
  r.x = (r.x+.5)/width;
  r.y = (r.y+.5)/height;

  return r;
}

const float infinity = 9.99999999999999999999999999e37;

vec3 intersect(float vertexIndex, vec4 rPos, vec4 rDir, vec3 lastHit, float triangleIndex, float minDist);

void main()
{
  vec4 rDir = texture2D(rayDir, gl_TexCoord[0].st);
  vec4 rPos = texture2D(rayPos, gl_TexCoord[0].st);
  vec4 triangleInfo = texture2D(triangleInfoTex, gl_TexCoord[0].st);


  gl_FragData[0] = rDir; //debug
  gl_FragData[1] = rPos; //debug
  gl_FragData[2] = triangleInfo; //debug
  //

  int triangleFlag = int(floor(rDir.w+.5));
  float gridIndex =  floor(rPos.w+.5);



  if(floor(triangleFlag+0.5) == ACTIVE_INTERSECT)
  {
    vec4 shadowInfo = texture2D(shadowInfoTex, gl_TexCoord[0].st);
    bool shadowActive = floor(shadowInfo.a+.5) == SHADOW_ACTIVE;
    float shadowTriangleIndex = floor(shadowInfo.r+.5);
    float shadowVertexIndex = floor(shadowInfo.g+.5);

    vec2 coord2D = index1Dto2D(gridIndex, maxTextureSize, gridSize);

//    float triangleIndex = floor(texture2D(grid, coord2D).a + .5);

    vec4 triangleIndexV = texture2D(grid, coord2D); //Debug
    float triangleIndex = floor(triangleIndexV.a+.5); //Debug
//    gl_FragData[3] = vec4(triangleIndexV.xyz, 1.5);//DEBUG

    coord2D = index1Dto2D(triangleIndex, maxTextureSize, triangleListSize);
    float vertexIndex = floor(texture2D(triangleList, coord2D).a + .5);


    vec3 lastHit = vec3(infinity, vertexIndex, triangleIndex);

    float minDist = 0.0;
    gl_FragData[3] = vec4(1,0,0, 1.5);//DEBUG
    if(shadowActive)
    {
      minDist = intersect(shadowVertexIndex, rPos, rDir, lastHit, shadowTriangleIndex, 0.0).r;
      if(minDist < infinity)
        gl_FragData[3] = vec4(0,0,1, 1.5);//DEBUG
      else
      {
        minDist = 0.0;
        gl_FragData[3] = vec4(0,1,0, 1.5);//DEBUG
      }
    }


    while(floor(vertexIndex+0.5) != -1.0)
    {
      lastHit = intersect(vertexIndex, rPos, rDir, lastHit, triangleIndex, minDist);
      triangleIndex = floor(triangleIndex+0.5) + 1.0;
      vec2 coord2D = index1Dto2D(triangleIndex, maxTextureSize, triangleListSize);
      vertexIndex = floor(texture2D(triangleList, coord2D).a + .5);
    }

    if(lastHit.r < infinity)
    {
      ///Set Ray State to Shading
      rDir.w = float(ACTIVE_SHADING);
      gl_FragData[0] = rDir;
      gl_FragData[1] = rPos;
//      gl_FragData[3] = vec4(0.5, 0., 0.0, .8);//DEBUG

//      vec3 fragPos = rPos.xyz + rDir.xyz*lastHit.r;
//      triangleInfo = vec4(fragPos, lastHit.g);
      triangleInfo = vec4(lastHit, 1.0);
      gl_FragData[2] = triangleInfo;
      return;
    }//else gl_FragData[3] = vec4(1., 0.5, 0, .8);//DEBUG

    rDir.w = float(ACTIVE_TRAVERSE_SEC);
/**/
  }
  else if(triangleFlag == ACTIVE_TRAVERSE_SEC){
    vec2 coord2D = index1Dto2D(gridIndex, maxTextureSize, gridSize);
    vec4 triangleIndexV = texture2D(grid, coord2D); //Debug
//    gl_FragData[3] = vec4(triangleIndexV.xyz, 0.5);//DEBUG
    gl_FragData[3] = vec4(0.5, 1.0, 1.0, .8);
  }
  else if(triangleFlag == ACTIVE_TRAVERSE){
    vec2 coord2D = index1Dto2D(gridIndex, maxTextureSize, gridSize);
    vec4 triangleIndexV = texture2D(grid, coord2D); //Debug
//    gl_FragData[3] = vec4(triangleIndexV.xyz, 0.5);//DEBUG
	gl_FragData[3] = vec4(0.3, 0.0, 0.5, .8);
  }

  ///Discard Pixel
  //   gl_FragData[0] = vec4(0., 1., 0., 1.);//DEBUG
  gl_FragData[0] = rDir;
  gl_FragData[1] = rPos;
  /**/
}


vec3 intersect(float vertexIndex, vec4 rPos, vec4 rDir, vec3 lastHit, float triangleIndex, float minDist)
{
  vec2 coord2D;

  coord2D = index1Dto2D(vertexIndex*3., maxTextureSize, vertexesSize);
  vec3 v0 = texture2D(vertexes, coord2D).xyz;
  coord2D = index1Dto2D(vertexIndex*3. + 1.0, maxTextureSize, vertexesSize);
  vec3 v1 = texture2D(vertexes, coord2D).xyz;
  coord2D = index1Dto2D(vertexIndex*3. + 2.0, maxTextureSize, vertexesSize);
  vec3 v2 = texture2D(vertexes, coord2D).xyz;

  vec3 edge1 = v1 - v0;
  vec3 edge2 = v2 - v0;

  vec3 pvec, qvec;
  float det, inv_det;
  float u,v,t;

  //find vectors for two edges sharing v0
  //begin calculating determinant � also used to calculate U param
  pvec = cross(rDir.xyz, edge2);

  //if determinant is near zero, ray lies in plane of triangle
  det = dot(pvec, edge1);

  if(det < 0.000001)
    return lastHit ;

  //calculate distance from v0 to ray origin
  vec3 tvec = rPos.xyz;
  tvec = tvec - v0;
  //   tvec = rPos.xyz � v0;

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
  t *= inv_det;
  //u*=inv_det;
  //v*=inv_det;

  if(t < lastHit.r && t > minDist)
    return vec3(t, vertexIndex, triangleIndex);
  else
    return lastHit;
}

/**/
