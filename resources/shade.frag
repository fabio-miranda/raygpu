uniform sampler2D rayDir;
uniform sampler2D rayPos;
uniform sampler2D triangleInfo;

uniform sampler2D vertexes;
uniform sampler2D normals;

uniform sampler2D materialTex;
uniform sampler2D colorTex;

uniform sampler1D lights;



uniform float maxTextureSize;

uniform float vertexesSize;
uniform float normalsSize;
uniform float materialSize;
uniform float lightsSize;



uniform vec3 eyePos;
uniform vec3 clearColor;

vec3 getInterpolatedNormal(float vertexIndex);
vec2 index1Dto2D(float index, float width, float size);


void calcDirLight(float i, vec3 N, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular);
void calcPointLight(float i, vec3 N, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular);
void calcSpotLight(float i, vec3 N, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular);

#define INACTIVE 0.0
#define ACTIVE_CALCULATEOUTVOXEL 1.0
#define ACTIVE_TRAVERSE 2.0
#define ACTIVE_INTERSECT 3.0
#define ACTIVE_TRAVERSE_SEC 4.0
#define ACTIVE_SHADING 5.0
#define OVERFLOW 6.0
#define DONE 7.0
#define ACTIVE_CALCULATEOUTVOXEL_SEC 8.0

//lightStruct
//{
//0|   difuse   //alpha == spotExponent
//1|   specular //alpha == enable or disable
//2|   pos      //w == type :0 directional, 1 point, 2 = spot
//3|   spot     //rgb == spotDir, a == spotAngle(rad)
//}

//materialStruct
//{
//0|   material //r = opacity, g = reflective, b = refractive, a = 1.0
//1|   diffuse // rgb = diffuse, a = 1.0
//2|   specular // rgb = specular, a = specularExp
//}

const vec3 defaultAmbientMaterial  = vec3(0.2, 0.2, 0.2);


vec4 lightSpecular;
vec4 lightPosition;
vec3 lightDir;


vec3 fragPos;
vec3 eyeDir;
struct material
{
  vec3 diffuse;
  vec3 specular;
  float shininess;
  float opacity;
  float reflective;
  float refractive;
}fragMaterial;

#define COLOR_NULL -1.0
#define COLOR_DONE 1.0
#define COLOR_MUL_LIGHT 2.0
#define COLOR_MUL_MAX_LIGHT 30.0

#define SHADOW_INACTIVE -1.0
#define SHADOW_ACTIVE 1.0

void main()
{
  vec4 rDir = texture2D(rayDir, gl_TexCoord[0].st);
  vec4 rPos = texture2D(rayPos, gl_TexCoord[0].st);
  float triangleFlag = floor(rDir.w+.5);

  vec4 color = texture2D(colorTex, gl_TexCoord[0].st);
  float colorTest = floor(color.a + .5);

  float numLights = floor(lightsSize/4.0+.5);

  if(colorTest >= COLOR_MUL_LIGHT && colorTest < COLOR_MUL_MAX_LIGHT)
  {
    vec4 triangleInfos = texture2D(triangleInfo, gl_TexCoord[0].st);
    float triangleIndex = floor(triangleInfos.b + .5);
    float vertexIndex = floor(triangleInfos.g + .5);

    fragPos = rPos.xyz;
    if(triangleFlag == ACTIVE_SHADING)
    {
      color.rgb *= .20;
      fragPos =  rPos.xyz + rDir.xyz*triangleInfos.r;
    }

    float i = colorTest - COLOR_MUL_LIGHT + 1;
    if(i < numLights)
    {
      vec4 lightSpecular = texture1D(lights, (i*4.0 + 1.0 + .5)/lightsSize);
      vec4 lightDirection = vec4(0,0,0,0);
      if(floor(lightSpecular.a + .5) != 0.0) //Is Light Enabled?
      {
        vec4 lightPosition = texture1D(lights, (i*4.0 + 2.0 + .5)/lightsSize);
        float lightType = floor(lightPosition.w+.5);
        if(lightType == 0.0) //Directional Light
        {
          lightDirection.xyz =  -lightPosition.xyz;
        }
        else if(lightType == 2.0) //Spot Light
        {
          lightDirection.xyz = -(lightPosition.xyz - fragPos);
        }
        else //Point Light
        {
          lightDirection.xyz = -(lightPosition.xyz - fragPos);
        }
        lightDirection.a = 1.0;
      }

      float lightIndex = i;

      rDir.xyz = normalize(-lightDirection.xyz);
      rPos.xyz = fragPos;
      rDir.w = float(ACTIVE_CALCULATEOUTVOXEL);

      gl_FragData[0] = rDir;
      gl_FragData[1] = rPos;
      gl_FragData[2] = texture2D(triangleInfo, gl_TexCoord[0].st);
      gl_FragData[3] = vec4(color.rgb, COLOR_MUL_LIGHT + lightIndex);
      gl_FragData[4] = vec4(triangleIndex, vertexIndex, lightIndex, SHADOW_ACTIVE);
    }else
    {
      rDir.w = float(DONE);
      gl_FragData[0] = rDir;
      gl_FragData[1] = rPos;
      gl_FragData[2] = texture2D(triangleInfo, gl_TexCoord[0].st);
      gl_FragData[3] = vec4(color.rgb, COLOR_DONE);
      gl_FragData[4] = vec4(SHADOW_INACTIVE);
    }
    return;
  }


  if(triangleFlag == ACTIVE_SHADING)
  {
    vec4 triangleInfos = texture2D(triangleInfo, gl_TexCoord[0].st);

    fragPos =  rPos.xyz + rDir.xyz*triangleInfos.r;
    float triangleIndex = floor(triangleInfos.b + .5);
    float vertexIndex = floor(triangleInfos.g + .5);

    vec3 normal = (getInterpolatedNormal(vertexIndex));

    vec3 ambient, diffuse, specular;
    ambient = defaultAmbientMaterial;
    diffuse = vec3(0, 0, 0);
    specular = vec3(0, 0, 0);
    eyeDir = -(eyePos - fragPos);

    vec2 coord2D = index1Dto2D(vertexIndex*3.0, maxTextureSize, materialSize);
    vec4 matInfo = texture2D(materialTex, coord2D);
    fragMaterial.opacity = matInfo.r;
    fragMaterial.reflective = floor(matInfo.g+.5);
    fragMaterial.refractive = matInfo.b;

    if(fragMaterial.reflective == 1.0)
    {
      rDir.xyz = reflect(rDir.xyz,normal.xyz);
      rPos.xyz = fragPos;
      rDir.w = float(ACTIVE_CALCULATEOUTVOXEL);

      gl_FragData[0] = rDir;
      gl_FragData[1] = rPos;
      gl_FragData[2] = texture2D(triangleInfo, gl_TexCoord[0].st);
      gl_FragData[3] = vec4(0,1,0,1);
      gl_FragData[4] = vec4(SHADOW_INACTIVE);
      return;
//    }else if(fragMaterial.opacity < 1.0)
//    {
//      rDir.xyz = normalize(refract(rDir.xyz, -(normal.xyz), fragMaterial.refractive));
//      rPos.xyz = fragPos;
//      rDir.w = float(ACTIVE_CALCULATEOUTVOXEL_SEC);
//
//      gl_FragData[0] = rDir;
//      gl_FragData[1] = rPos;
//      gl_FragData[2] = texture2D(triangleInfo, gl_TexCoord[0].st);
//      gl_FragData[3] = vec4(0,0,1,1);
//        gl_FragData[4] = vec4(SHADOW_INACTIVE);
//      return;
    }

    coord2D = index1Dto2D(vertexIndex*3.0 + 1.0, maxTextureSize, materialSize);
    fragMaterial.diffuse = texture2D(materialTex, coord2D).rgb;

    coord2D = index1Dto2D(vertexIndex*3.0 + 2.0, maxTextureSize, materialSize);
    matInfo = texture2D(materialTex, coord2D);
    fragMaterial.specular = matInfo.rgb;
    fragMaterial.shininess = matInfo.a;

    vec4 firstLightDir;
    firstLightDir.a = -1.0;
    for(float i = 0.0; i<numLights; i += 1.0)
    {
      lightSpecular = texture1D(lights, (i*4.0 + 1.0 + .5)/lightsSize);
      if(floor(lightSpecular.a + .5) != 0.0) //Is Light Enabled?
      {
        lightPosition = texture1D(lights, (i*4.0 + 2.0 + .5)/lightsSize);
        float lightType = floor(lightPosition.w+.5);
        if(lightType == 0.0) //Directional Light
        {
          lightDir =  -lightPosition.xyz;
          calcDirLight(i, normal, ambient, diffuse, specular);
        }
        else if(lightType == 2.0) //Spot Light
        {
          lightDir = -(lightPosition.xyz - fragPos);
          calcSpotLight(i, normal, ambient, diffuse, specular);
        }
        else //Point Light
        {
          lightDir = -(lightPosition.xyz - fragPos);
          calcPointLight(i, normal, ambient, diffuse, specular);
        }

        if(firstLightDir.a == -1.0) //ONLY ONCE
        {
          firstLightDir.a = i;
          firstLightDir.xyz = lightDir;
        }
      }
    }

    ///Set Ray State to LIGHT
    float lightIndex  = firstLightDir.a;

    rDir.xyz = normalize(-firstLightDir.xyz);
    rPos.xyz = fragPos;
    rDir.w = float(ACTIVE_CALCULATEOUTVOXEL);

    gl_FragData[0] = rDir;
    gl_FragData[1] = rPos;
    gl_FragData[2] = texture2D(triangleInfo, gl_TexCoord[0].st);
    gl_FragData[3] = vec4(ambient + diffuse + specular, COLOR_MUL_LIGHT + lightIndex);
    gl_FragData[4] = vec4(triangleIndex, vertexIndex, lightIndex, SHADOW_ACTIVE);


    ///Set Ray State to Done
//    rDir.w = float(DONE);
//    gl_FragData[0] = rDir;
//    gl_FragData[1] = rPos;
//    gl_FragData[2] = texture2D(triangleInfo, gl_TexCoord[0].st);
//    gl_FragData[3] = vec4(ambient + diffuse + specular, COLOR_MUL_LIGHT);
//    gl_FragData[3] = vec4(vec3(0,1,1), COLOR_DONE);
  }
  else if(triangleFlag == DONE)
  {
    gl_FragData[0] = rDir;
//    gl_FragData[1] = rPos;
//    gl_FragData[2] = texture2D(triangleInfo, gl_TexCoord[0].st);
    gl_FragData[3] = vec4(color.rgb, COLOR_DONE) ;
//    gl_FragData[3] = vec4(vec3(0,0,1), COLOR_DONE);
    gl_FragData[4] = vec4(SHADOW_INACTIVE);

  }
//  else if(triangleFlag == ACTIVE_TRAVERSE ||
//  triangleFlag == ACTIVE_CALCULATEOUTVOXEL ||
//  triangleFlag == ACTIVE_TRAVERSE_SEC ||
//  triangleFlag == ACTIVE_TRAVERSE_SEC ||
//  triangleFlag == ACTIVE_INTERSECT  )
//  {
//
//    gl_FragData[0] = rDir;
//    gl_FragData[1] = rPos;
//    gl_FragData[2] = texture2D(triangleInfo, gl_TexCoord[0].st);
//    gl_FragData[3] = vec4(clearColor, COLOR_DONE);
//  //  gl_FragData[3] = vec4(vec3(1,0,0), COLOR_DONE);
//    gl_FragData[4] = vec4(SHADOW_INACTIVE);
//  }
  else
  {
    ///Discard Pixel
    gl_FragData[0] = rDir;
    gl_FragData[1] = rPos;
    gl_FragData[2] = texture2D(triangleInfo, gl_TexCoord[0].st);
//    if(colorTest >= COLOR_MUL_LIGHT && colorTest < COLOR_MUL_MAX_LIGHT)
//      gl_FragData[3] = vec4(color.rgb, COLOR_DONE);
//    else
      gl_FragData[3] = vec4(clearColor, COLOR_DONE);
//    gl_FragData[3] = vec4(vec3(0,1,0), COLOR_DONE);
    gl_FragData[4] = vec4(SHADOW_INACTIVE);
  }
  /**/
}

void calcDirLight(float i, vec3 N, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular)
{
  vec3 L = normalize(lightDir);
  vec3 H = normalize(L + normalize(eyeDir));

  float NdotL = max(0.0, dot(N, L));
  if ( NdotL > 0.0 )
  {
    vec3 lightDiffuse = texture1D(lights, (i*4.0 + .5)/lightsSize).rgb;
    float att = 1.0; //future work
    float NdotH = max(0.0, dot(N, H));

    diffuse += fragMaterial.diffuse * lightDiffuse * NdotL;

    specular += fragMaterial.specular * lightSpecular.rgb * pow(NdotH, fragMaterial.shininess);

  }
}

void calcPointLight(float i, vec3 N, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular)
{
  vec3 L = normalize(lightDir);
  vec3 H = normalize(L + normalize(eyeDir));


  float NdotL = max(0.0, dot(N, L));
  if ( NdotL > 0.0 )
  {
    vec3 lightDiffuse = texture1D(lights, (i*4.0 + .5)/lightsSize).rgb;
    float att = 1.0; //future work
    float NdotH = max(0.0, dot(N, H));

    diffuse += fragMaterial.diffuse * lightDiffuse * NdotL;

		specular += fragMaterial.specular * lightSpecular.rgb * pow(NdotH, fragMaterial.shininess);
	}
}

void calcSpotLight(float i, vec3 N, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular)
{
  vec3 L = normalize(lightDir);
  vec3 H = normalize(L + normalize(eyeDir));

  float NdotL = max(0.0, dot(N, L));
  if ( NdotL > 0.0 )
  {
    vec4 lightSpotInfo = texture1D(lights, (i*4.0 + 3.0 + .5)/lightsSize);
    vec3 lightSpotDir = -lightSpotInfo.rgb;
    float lightSpotAngle = cos(lightSpotInfo.a);

    float spotEffect = dot(normalize(lightSpotDir),-L);
    if (spotEffect > lightSpotAngle)
    {
      vec4 lightDiffuse = texture1D(lights, (i*4.0 + .5)/lightsSize);
      float att = 1.0; //future work
      float NdotH = max(0.0, dot(N, H));

      spotEffect = pow(spotEffect, lightDiffuse.a);
      diffuse += fragMaterial.diffuse * lightDiffuse.rgb * NdotL * spotEffect;

      specular += fragMaterial.specular * lightSpecular.rgb * pow(NdotH, fragMaterial.shininess)*spotEffect;
    }
  }
}

vec3 getInterpolatedNormal(float vertexIndex)
{
  vec2 coord2D = index1Dto2D(vertexIndex*3.0, maxTextureSize, normalsSize);
  vec3 normal1 = texture2D(normals, coord2D).xyz;
  coord2D = index1Dto2D(vertexIndex*3.0+1.0, maxTextureSize, normalsSize);
  vec3 normal2 = texture2D(normals, coord2D).xyz;
  coord2D = index1Dto2D(vertexIndex*3.0+2.0, maxTextureSize, normalsSize);
  vec3 normal3 = texture2D(normals, coord2D).xyz;

  coord2D = index1Dto2D(vertexIndex*3., maxTextureSize, vertexesSize);
  vec3 v1 = texture2D(vertexes, coord2D).xyz;
  coord2D = index1Dto2D(vertexIndex*3. + 1.0, maxTextureSize, vertexesSize);
  vec3 v2 = texture2D(vertexes, coord2D).xyz;
  coord2D = index1Dto2D(vertexIndex*3. + 2.0, maxTextureSize, vertexesSize);
  vec3 v3 = texture2D(vertexes, coord2D).xyz;

  vec3 U =  v1-v2;
  vec3 V =  v3-v2;
  vec3 N =  fragPos-v2;

  float dU = length(U);
  float dV = length(V);
  float dN = length(N);

  N = normalize(N);
  U = normalize(U);

  float cost = dot(N,U);
  if (cost < 0.0) cost = 0.0;
  if (cost > 1.0) cost = 1.0;

  float t = acos(cost);

  float distY = 0.0, distX = 0.0;
  distX = dN * cos(t);
  distY = dN * sin(t);

  float u = distX/ dU;
  float v = distY/ dV;

  normal1 = normalize(normal1);
  normal2 = normalize(normal2);
  normal3 = normalize(normal3);

  float nx = -( (1.0 - (u + v)) * normal2.x +
    normal1.x * u +
    normal3.x * v);
  float ny = -( (1.0 - (u + v)) * normal2.y +
    normal1.y * u +
    normal3.y * v);
  float nz = -( (1.0 - (u + v)) * normal2.z +
    normal1.z * u +
    normal2.z * v);


  vec3 normal = normalize(vec3(nx, ny, nz));
  return normal;
}

vec2 index1Dto2D(float index, float width, float size)
{
  float height = float(int(size/width))+1.0;
  vec2 r = vec2(float(mod(index,width)), float(int(index/width)));
  r.x = (r.x+.5)/width;
  r.y = (r.y+.5)/height;

  return r;
}
