uniform vec3 eyeDir;
uniform vec3 eyePos;
uniform vec3 eyeRight;
uniform float nearPlane;
uniform vec3 eyeUp;
uniform vec3 bbMin;
uniform vec3 bbMax;
uniform vec3 nearPlaneSize;
uniform vec3 screenSize;
uniform vec3 gridVoxelSize;
uniform vec3 gridSize;
uniform float gridArraySize;

//Ray states (stored on rayDir.a)
#define INACTIVE 0.0
#define ACTIVE_CALCULATEOUTVOXEL 1.0
#define ACTIVE_TRAVERSE 2.0
#define ACTIVE_INTERSECT 3.0
#define ACTIVE_TRAVERSE_SEC 4.0
#define ACTIVE_SHADING 5.0
#define OVERFLOW 6.0
#define DONE 7.0

//Ray voxel (stored on rayPos.a)


#define COLOR_NULL -1.0
#define COLOR_DONE 1.0
#define COLOR_MUL_LIGHT 2.0


#define SHADOW_INACTIVE -1.0
#define SHADOW_ACTIVE 1.0

vec3 getRay(vec2 screenPos){

	vec3 u = ((screenPos.x)) * normalize(eyeRight) * (nearPlaneSize.x);
	vec3 v = ((screenPos.y)) * normalize(eyeUp) * (nearPlaneSize.y);
	vec3 point = eyePos + nearPlane * normalize(eyeDir) + u + v;
	point = point - (nearPlaneSize.x/2.0) * normalize(eyeRight) - (nearPlaneSize.y/2.0) * normalize(eyeUp);

	/*
	vec3 u = (screenPos.x/(screenSize.x)) * eyeRight;
	vec3 v = (screenPos.y/(screenSize.y)) * eyeUp;
	vec3 point = u + v;
	*/

	return normalize(point - eyePos);

}

bool insideGrid(vec3 position){

	if(position.x >= bbMin.x && position.y >= bbMin.y && position.z >= bbMin.z
		&& position.x <= bbMax.x && position.y <= bbMax.y && position.z <= bbMax.z)
		return true;
	else
		return false;

}

bool hitGrid(vec3 position, vec3 direction, out float intersectionMin, out float intersectionMax){

	//Woo's method for ray/AABB collision test

	intersectionMin = 0.0;
	intersectionMax = 0.0;
	float auxMin = 0.0;
	float auxMax = 0.0;
	bool hit = true;

	if(direction.x >= 0.0){
		intersectionMin = (bbMin.x - position.x) / direction.x;
		intersectionMax = (bbMax.x - position.x) / direction.x;
	}
	else{
		intersectionMin = (bbMax.x - position.x) / direction.x;
		intersectionMax = (bbMin.x - position.x) / direction.x;
	}


	if(direction.y >= 0.0){
		auxMin = (bbMin.y - position.y) / direction.y;
		auxMax = (bbMax.y - position.y) / direction.y;
	}
	else{
		auxMin = (bbMax.y - position.y) / direction.y;
		auxMax = (bbMin.y - position.y) / direction.y;
	}

	if(intersectionMin > auxMax || auxMin > intersectionMax) hit = false;

	if(auxMin > intersectionMin) intersectionMin = auxMin;
	if(auxMax < intersectionMax) intersectionMax = auxMax;

	if(direction.z >= 0.0){
		auxMin = (bbMin.z - position.z) / direction.z;
		auxMax = (bbMax.z - position.z) / direction.z;
	}
	else{
		auxMin = (bbMax.z - position.z) / direction.z;
		auxMax = (bbMin.z - position.z) / direction.z;
	}

	if(intersectionMin > auxMax || auxMin > intersectionMax) hit = false;

	if(auxMin > intersectionMin) intersectionMin = auxMin;
	if(auxMax < intersectionMax) intersectionMax = auxMax;

	return hit;
}

vec3 findVoxel(vec3 rayPos){

	vec3 index = vec3(-1.0, -1.0, -1.0);

	//index = (floor(rayPos+0.5) - floor(bbMin+0.5)) / floor(gridVoxelSize+0.5);
	//index = (rayPos - bbMin) / gridVoxelSize;
//	index = (floor(rayPos+0.5) - bbMin) / gridVoxelSize;
	index = (rayPos - bbMin) / gridVoxelSize;
	index = trunc(index);

	if(index.x >= gridSize.x)
		index.x = gridSize.x - 1.0;
	if(index.y >= gridSize.y)
		index.y = gridSize.y - 1.0;
	if(index.z >= gridSize.z)
		index.z = gridSize.z - 1.0;

	return index;

}

float findVoxelLinearArray(vec3 voxelIndex){
	return voxelIndex.x*gridSize.y*gridSize.z + voxelIndex.y*gridSize.z + voxelIndex.z;

}

vec3 findVoxelPosition(vec3 voxelIndex){
	return voxelIndex * gridVoxelSize + bbMin;
}

vec3 findIntersectionOutVoxel(vec3 rayPos, vec3 rayDir, vec3 voxelIndex){

	vec3 intersectionOut = vec3(0.0, 0.0, 0.0);

	if(rayDir.x < 0.0)
		intersectionOut.x = (findVoxelPosition(voxelIndex).x - rayPos.x) / rayDir.x;
	if(rayDir.x > 0.0)
		intersectionOut.x = (findVoxelPosition(voxelIndex + vec3(1, 0, 0)).x - rayPos.x) / rayDir.x;

	if(rayDir.y < 0.0)
		intersectionOut.y = (findVoxelPosition(voxelIndex).y - rayPos.y) / rayDir.y;
	if(rayDir.y > 0.0)
		intersectionOut.y = (findVoxelPosition(voxelIndex + vec3(0, 1, 0)).y - rayPos.y) / rayDir.y;

	if(rayDir.z < 0.0)
		intersectionOut.z = (findVoxelPosition(voxelIndex).z - rayPos.z) / rayDir.z;
	if(rayDir.z > 0.0)
		intersectionOut.z = (findVoxelPosition(voxelIndex + vec3(0, 0, 1)).z - rayPos.z) / rayDir.z;

	return intersectionOut;


}

void main(){

	vec4 rayDir = vec4(getRay(gl_TexCoord[0].xy), 1.0);
	vec4 rayPos = vec4(eyePos, -1.0);
	vec3 voxelIndex = vec3(0, 0, 0);
	vec3 intersectionOut = vec3(0,0,0);


	float intersectionMin = 0.0;
	float intersectionMax = 0.0;
	//Sets the ray state
	//The camera is inside the grid
	float breakpoint = 0.0;

	if(insideGrid(rayPos.xyz)){
		rayDir.a = ACTIVE_CALCULATEOUTVOXEL;
		voxelIndex = findVoxel(rayPos.xyz);
		rayPos.a = findVoxelLinearArray(voxelIndex.xyz);
	}
	else{

		//Check if the ray hits the grid
		//P1 = P0 + t*dir
		//intersectionMin is the minimum distance from the eye to the bounding box
		//intersectionMax is the maximum distance from the eye to the bounding box
		bool hit = hitGrid(rayPos.xyz, rayDir.xyz, intersectionMin, intersectionMax);
		if(hit && intersectionMin > 0.0){
			rayDir.a = ACTIVE_CALCULATEOUTVOXEL;
			rayPos.xyz = rayPos.xyz + intersectionMin * rayDir.xyz;
			voxelIndex = findVoxel(rayPos.xyz);
			rayPos.a = findVoxelLinearArray(voxelIndex.xyz);

			//intersectionOut = findIntersectionOutVoxel(rayPos.xyz, rayDir.xyz, voxelIndex.xyz);
			//posIntersectionOut = rayPos.xyz + posIntersectionOut * rayDir.xyz;

			breakpoint = 1.0;

			if(rayPos.a >= gridArraySize)
				hit = false;
		}

		if(hit == false){
			rayPos = vec4(0.0, 0.0 ,0.0, -1.0);
			rayDir.a = INACTIVE;
			breakpoint = -1.0;
		}
	}
  gl_FragData[0] = vec4(-1.0, -1.0, -1.0, COLOR_NULL);
  gl_FragData[1] = vec4(-1.0, -1.0, -1.0, SHADOW_INACTIVE);
  gl_FragData[2] = rayPos;
	gl_FragData[3] = rayDir;

//	gl_FragData[0] = vec4(normalize(rayPos.xyz), 0.5);
//	gl_FragData[0] = vec4((vec3(length(rayPos.xyz-eyePos.xyz)/25.)), 1.5);
//	gl_FragData[0] = vec4(normalize(vec3(intersectionMin)), 1.5);
	//gl_FragData[0] = vec4(vec3(breakpoint), 0.5);
//	gl_FragData[0] = vec4(normalize(rayPos.xyz + intersectionMax * rayDir.xyz;), 0.5);
	//gl_FragData[0] = vec4(vec3(rayPos.w/(gridSize.x * gridSize.y * gridSize.z)), 1.0);
	//gl_FragData[0] = vec4(normalize(voxelIndex.xyz), 1.0);
	//gl_FragData[0] = vec4((normalize(rayDir.xyz) + 1.0)/2.0, 0.5);
	/**/
	//gl_FragData[0] = vec4(normalize(rayPos.xyz), 1.0);

	//gl_FragData[1] = vec4(normalize(rayDir.xyz), 1.0);

	//gl_FragData[2] = vec4(vec3(intersectionMin),1.0);
	//gl_FragData[2] = vec4(intersectionMin);
	//gl_FragData[3] = vec4(normalize(intersectionOut), 1.0);
//	gl_FragData[3] = vec4(intersectionOut, 1.0);
	//gl_FragData[3] = vec4(normalize(voxelIndex.xyz), 0.8);

	//gl_FragData[3] = vec4(normalize(rayPos.xyz), 1);
	//gl_FragData[3] = vec4(vec3(normalize(voxelIndex.xyz)), .8);
	//gl_FragData[3] = vec4(vec3(rayPos.a/1000.0), 1.0);
	//gl_FragData[3] = vec4(vec3(intersectionMin),1.0);
	//gl_FragData[3] = vec4(intersectionOut, 1.0);

	//gl_FragData[3] = vec4(vec3(breakpoint), 0.5);


	//if(rayPos.a == 1)
		//gl_FragData[3] = vec4(vec3(normalize(voxelIndex.xyz)), 0.0);
	//else
		//gl_FragData[3] = vec4(vec3(normalize(voxelIndex.xyz)), 1.0);

}
