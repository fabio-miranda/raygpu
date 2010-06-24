uniform sampler2D samplerRayPos;
uniform sampler2D samplerRayDir;
uniform sampler2D samplerGridIntersectionMax;
uniform sampler2D samplerGridIntersectionMin;
uniform sampler1D samplerGrid;

uniform vec3 gridSize;
uniform vec3 gridVoxelSize;
uniform float gridArraySize;
uniform vec3 bbMin;
uniform vec3 bbMax;

//Ray states (stored on rayDir.a)
#define INACTIVE 0.0
#define ACTIVE_TRAVERSE 1.0
#define ACTIVE_INTERSECT 2.0
#define OVERFLOW 3.0

//Voxel index (stored on rayPos.a)

float findVoxelLinearArray(vec3 voxelIndex){

	return voxelIndex.x*gridSize.y*gridSize.z + voxelIndex.y*gridSize.z + voxelIndex.z;

}

vec3 findVoxel(vec3 rayPos){

	vec3 index = vec3(-1.0, -1.0, -1.0);

	index = (rayPos - bbMin) / gridVoxelSize;
	index.x = float(int(index.x));
	index.y = float(int(index.y));
	index.z = float(int(index.z));

	return index;

}


void main(){

	vec4 rayDir = texture2D(samplerRayDir, gl_TexCoord[0].st);
	vec4 rayPos = texture2D(samplerRayPos, gl_TexCoord[0].st);

	//vec4 gridIndex = texture1D(samplerGrid, findVoxelLinearArray(findVoxel(rayPos.xyz))/gridArraySize);
	vec4 gridIndex = texture1D(samplerGrid, floor(rayPos.a+.5)/gridArraySize);
	vec4 gridIntersectionMax = texture2D(samplerGridIntersectionMax, gl_TexCoord[0].st);

	if(gridIndex.x >= gridSize.x || gridIndex.y >= gridSize.y || gridIndex.z >= gridSize.z
		|| gridIndex.x < 0.0 || gridIndex.y < 0.0 || gridIndex.z < 0.0)
	{
		gl_FragData[0] = vec4(0,0,0,1);
		gl_FragData[3] = vec4(0, 0, 0, 1);
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
	else if(floor(rayDir.a + 0.5) == ACTIVE_TRAVERSE){

		if(floor(gridIndex.a+0.5) > 0.0){
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

			if(gridIntersectionMax.x < gridIntersectionMax.y && gridIntersectionMax.x < gridIntersectionMax.z){
				gridIndex.x += step.x;
				gridIntersectionMax.x += delta.x;
			}
			else if((gridIntersectionMax.x < gridIntersectionMax.y && gridIntersectionMax.x > gridIntersectionMax.z) || (gridIntersectionMax.x > gridIntersectionMax.y && gridIntersectionMax.y > gridIntersectionMax.z)){
				gridIndex.z += step.z;
				gridIntersectionMax.z += delta.z;
			}
			else if(gridIntersectionMax.x > gridIntersectionMax.y && gridIntersectionMax.y < gridIntersectionMax.z){
				gridIndex.y += step.y;
				gridIntersectionMax.y += delta.y;
			}

			if(gridIndex.x >= gridSize.x || gridIndex.y >= gridSize.y || gridIndex.z >= gridSize.z)
			{
				rayDir.a = INACTIVE;
				rayPos.a = findVoxelLinearArray(gridIndex.xyz);
				gl_FragData[3] = vec4(1, 1, 1, 0.8);
			}
			else if(gridIndex.x < 0.0 || gridIndex.y < 0.0 || gridIndex.z < 0.0){
				rayDir.a = INACTIVE;
				rayPos.a = findVoxelLinearArray(gridIndex.xyz);
				gl_FragData[3] = vec4(0.5, 0.5, 0.5, 0.8);
			}
			else{

				//Keep traversing
				float gridLinearIndex = findVoxelLinearArray(gridIndex.xyz);
				gridIndex = (texture1D(samplerGrid, floor((gridLinearIndex+.5))/gridArraySize));

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
	//gl_FragData[0] = vec4(normalize(rayPos.xyz), 0.8);
	gl_FragData[1] = rayDir; //untouched, except for rayDir.a
	gl_FragData[2] = gridIntersectionMax;
	//gl_FragData[3] = vec4(normalize(gridIndex.xyz), 0.9);
	

}
