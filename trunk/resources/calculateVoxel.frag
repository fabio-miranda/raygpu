uniform sampler2D samplerRayPos;
uniform sampler2D samplerRayDir;

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

	vec4 rayDir = texture2D(samplerRayDir, gl_TexCoord[0].st);
	vec4 rayPos = texture2D(samplerRayPos, gl_TexCoord[0].st);
	vec3 voxelIndex = vec3(0, 0, 0);
	vec3 intersectionOut = vec3(0,0,0);


	if(rayDir.a == ACTIVE_CALCULATEOUTVOXEL){
		rayDir.a = ACTIVE_TRAVERSE;
		voxelIndex = findVoxel(rayPos.xyz);
		rayPos.a = findVoxelLinearArray(voxelIndex.xyz);

		intersectionOut = findIntersectionOutVoxel(rayPos.xyz, rayDir.xyz, voxelIndex.xyz);

	}

	gl_FragData[0] = rayPos;
	gl_FragData[1] = rayDir;
	gl_FragData[2] = vec4(intersectionOut, 1.0);
	gl_FragData[3] = vec4(normalize(intersectionOut), 1.0);

}
