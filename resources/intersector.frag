uniform sampler2D rayPos;
uniform sampler2D rayDir;

uniform sampler1D grid;
uniform sampler2D vertexes;
uniform sampler1D triangleList;



uniform float maxTextureSize;

uniform float gridSize;
uniform float triangleListSize;
uniform float vertexesSize;

#define INACTIVE 0
#define ACTIVE_TRAVERSE 1
#define ACTIVE_INTERSECT 2
#define ACTIVE_SHADING 3
#define DONE 4
#define OVERFLOW 5

vec2 index1Dto2D(float index, float width, float size)
{
  float height = float(trunc(size/width))+1.0;
  vec2 r = vec2(float(mod(index,width)), float(trunc(index/width)));
  r.x = (r.x+.5)/width;
  r.y = (r.y+.5)/height;

  return r;
}

const float infinity = 9.99999999999999999999999999e37;

vec3 intersect(float vertexIndex, vec4 rPos, vec4 rDir, vec3 lastHit, float triangleIndex);

void main()
{
  vec4 rDir = texture2D(rayDir, gl_TexCoord[0].st);
  vec4 rPos = texture2D(rayPos, gl_TexCoord[0].st);

  gl_FragData[0] = rDir; //debug
  gl_FragData[1] = rPos; //debug
  //

  int triangleFlag = int(floor(rDir.w+.5));
  float gridIndex =  floor(rPos.w+.5);


  gl_FragData[2] = vec4(0., 1., 1., 0.5); //DEBUG
  if(triangleFlag == ACTIVE_INTERSECT)
  {
    float triangleIndex = floor(texture1D(grid, (gridIndex + .5)/gridSize).a + .5);
    float vertexIndex = floor(texture1D(triangleList, (triangleIndex + .5)/triangleListSize).a + .5);
    vec3 lastHit = vec3(infinity, vertexIndex, triangleIndex);

    gl_FragData[2] = vec4(7., 0., 0., 0.5);//DEBUG

    while(vertexIndex != -1.0)
    {
      lastHit = intersect(vertexIndex, rPos, rDir, lastHit, triangleIndex);
      triangleIndex++;
      vertexIndex = floor(texture1D(triangleList, (triangleIndex + .5)/triangleListSize).a + .5);
    }

    if(lastHit.r < infinity)
    {
      ///Set Ray State to Shading
      rDir.w = float(ACTIVE_SHADING);
//      gl_FragData[0] = rDir;
      gl_FragData[2] = vec4(0., 0., lastHit.b, 0.5);//DEBUG

      vec3 fragPos = rPos.xyz + rDir.xyz*lastHit.r;
      vec4 triangleInfo = vec4(fragPos, lastHit.b);
//      gl_FragData[1] = triangleInfo;
      return;
    }
  }

  ///Discard Pixel
  //   gl_FragData[0] = vec4(0., 1., 0., 1.);//DEBUG
//  gl_FragData[0] = rDir;
//  gl_FragData[1] = vec4(-1, -1, -1, -1);
  /**/
}


vec3 intersect(float vertexIndex, vec4 rPos, vec4 rDir, vec3 lastHit, float triangleIndex)
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
  t *= inv_det;
  //u*=inv_det;
  //v*=inv_det;

  if(t < lastHit.r)
    return vec3(t, vertexIndex, triangleIndex);
  else
    return lastHit;
}

/**/
