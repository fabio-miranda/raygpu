uniform sampler2D samplerRayPos;
uniform sampler2D samplerRayDir;
uniform sampler2D samplerGridIntersectionMax;
uniform sampler2D samplerGrid;

uniform vec3 gridSize;
uniform vec3 gridVoxelSize;
uniform float gridArraySize;
uniform vec3 bbMin;
uniform vec3 bbMax;

uniform float maxTextureSize;

//Ray states (stored on rayDir.a)
#define INACTIVE 0.0
#define ACTIVE_CALCULATEOUTVOXEL 1.0
#define ACTIVE_TRAVERSE 2.0
#define ACTIVE_INTERSECT 3.0
#define ACTIVE_TRAVERSE_SEC 4.0
#define ACTIVE_SHADING 5.0
#define OVERFLOW 6.0
#define DONE 7.0


vec2 index1Dto2D(float index, float width, float size)
{
  float height = float(trunc(size/width))+1.0;
  vec2 r = vec2(float(mod(index,width)), float(trunc(index/width)));
  r.x = (r.x+.5)/width;
  r.y = (r.y+.5)/height;

  return r;
}

//Voxel index (stored on rayPos.a)

float findVoxelLinearArray(vec3 voxelIndex){

	return voxelIndex.x*gridSize.y*gridSize.z + voxelIndex.y*gridSize.z + voxelIndex.z;

}

vec3 findVoxel(vec3 rayPos){

	vec3 index = vec3(-1.0, -1.0, -1.0);

	index = (floor(rayPos-0.5) - bbMin) / gridVoxelSize;
	index = trunc(index);

	return index;

}

vec3 findVoxelPosition(vec3 voxelIndex){
	return voxelIndex * gridVoxelSize + bbMin;
}


void main(){

	vec4 rayDir = texture2D(samplerRayDir, gl_TexCoord[0].st);
	vec4 rayPos = texture2D(samplerRayPos, gl_TexCoord[0].st);
	//float a = (floor(rayPos.a+0.5)+0.5)/(gridArraySize);
	vec2 coord2D = index1Dto2D(floor(rayPos.a+0.5), maxTextureSize, gridArraySize);
	vec4 gridIndex = texture2D(samplerGrid, coord2D );
//	vec4 gridIndex = texture1D(samplerGrid, a);
	vec4 gridIntersectionMax = texture2D(samplerGridIntersectionMax, gl_TexCoord[0].st);
	//vec4 gridIntersectionMax = vec4(findIntersectionOutVoxel(rayPos.xyz, rayDir.xyz, gridIndex.xyz), 1.0);

	//vec4 gridIndex = texture1D(samplerGrid, findVoxelLinearArray(findVoxel(rayPos.xyz))/gridArraySize);
	/*
	if(floor(rayPos.a+0.5) < 0)
		return;
	*/







	//gl_FragData[3] = vec4(normalize(gridIndex.xyz), 1.0);
	//gl_FragData[3] = vec4(vec3(rayPos.a/1000.0), 1.0);
	//return;

	if(gridIndex.x >= gridSize.x || gridIndex.y >= gridSize.y || gridIndex.z >= gridSize.z
		|| gridIndex.x < 0.0 || gridIndex.y < 0.0 || gridIndex.z < 0.0
		|| rayDir.a == INACTIVE)
	{
		gl_FragData[0] = vec4(0,0,0,INACTIVE);
		gl_FragData[3] = vec4(0, 0, 1, .8);
		return;
		//discard;
	}
	/*
	if(floor(gridIndex.a+0.5) > 0 && floor(rayDir.a+0.5) == ACTIVE_TRAVERSE){
		gl_FragData[3] = vec4(0.0, 1.0, 0.0, 0.8);
	}
	else if(floor(rayDir.a+0.5) == ACTIVE_TRAVERSE){
		gl_FragData[3] = vec4(0.0, 0.0, 1.0, 0.8);
	}
	*/
	if(floor(rayDir.a + 0.5) == ACTIVE_INTERSECT){
		gl_FragData[3] = vec4(1.0, 0.0, 0.0, 0.8);
	}
	else if(floor(rayDir.a + 0.5) == ACTIVE_TRAVERSE ||
           floor(rayDir.a + 0.5) == ACTIVE_TRAVERSE_SEC){

		if(floor(gridIndex.a+0.5) > -1.0 && floor(rayDir.a + 0.5) == ACTIVE_TRAVERSE){
			rayDir.a = ACTIVE_INTERSECT;
			gl_FragData[3] = vec4(0.0, 1.0, 0.0, 0.8);
		}
		else{ //Traverse

			//rayPos.xyz = gridIntersectionMax.xyz;
			vec3 delta = abs(gridVoxelSize.xyz / rayDir.xyz);
			vec3 step = vec3(1.0, 1.0, 1.0);
			if(rayDir.x < 0.0) step.x = -1.0;
			if(rayDir.y < 0.0) step.y = -1.0;
			if(rayDir.z < 0.0) step.z = -1.0;

			bool cmpXY = gridIntersectionMax.x - gridIntersectionMax.y < 0.1;
			bool cmpXZ = gridIntersectionMax.x - gridIntersectionMax.z < 0.1;
			bool cmpYZ = gridIntersectionMax.y - gridIntersectionMax.z < 0.1;

			if(cmpXY && cmpXZ){
				gridIndex.x = floor(gridIndex.x + step.x + 0.5);
				gridIntersectionMax.x += delta.x;
			}
			else if((cmpXY && !cmpXZ) || (!cmpXY && !cmpYZ)){
				gridIndex.z = floor(gridIndex.z + step.z + 0.5);
				gridIntersectionMax.z += delta.z;
			}
			else if(!cmpXY && cmpYZ){
				gridIndex.y = floor(gridIndex.y + step.y + 0.5);
				gridIntersectionMax.y += delta.y;
			}
			/*
			if(gridIndex.x  > 0.0 || gridIndex.y > 0.0 || gridIndex.z > 0.0)
			{
				rayDir.a = INACTIVE;
				rayPos.a = findVoxelLinearArray(gridIndex.xyz);
				gl_FragData[3] = vec4(1, 1, 1, 0.8);
			}
			*/
			gridIndex = floor(gridIndex+0.5);
			if(gridIndex.x >= gridSize.x || gridIndex.y >= gridSize.y || gridIndex.z >= gridSize.z
				|| gridIndex.x < 0.0 || gridIndex.y < 0.0 || gridIndex.z < 0.0){
				rayDir.a = INACTIVE;
				rayPos.a = findVoxelLinearArray(gridIndex.xyz);
				gl_FragData[3] = vec4(0.5, 0.5, 0.5, 0.8);
			}
			else{

				//Keep traversing
				float gridLinearIndex = findVoxelLinearArray(gridIndex.xyz);
				rayDir.a = ACTIVE_TRAVERSE;
				gl_FragData[3] = vec4(1, 0, 1, 0.8);
				rayPos.a = gridLinearIndex;


				//gl_FragData[3] = vec4(vec3(gridLinearIndex/1000.0), 1.0);
				//gl_FragData[3] = vec4(normalize(gridIndex.xyz), 1.0);
				//gl_FragData[3] = vec4((normalize(rayPos.xyz)+1.0)/2.0, 1.0);
			}
		}
	}




//   gl_FragData[0] = rayPos;
	gl_FragData[0] = rayPos; //untouched, except for rayPos.a
	//gl_FragData[0] = vec4(normalize(rayDir.xyz), 0.8);
	gl_FragData[1] = rayDir; //untouched, except for rayDir.a
	gl_FragData[2] = gridIntersectionMax;
	//gl_FragData[0] = vec4(vec3(gridIndex.a/gridArraySize), 0.9);

	//gl_FragData[3] = vec4(normalize(gridIntersectionMax.xyz), 0.8);

}
