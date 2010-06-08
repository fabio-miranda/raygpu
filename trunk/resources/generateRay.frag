uniform vec3 eyeDir;
uniform vec3 eyePos;
uniform vec3 eyeRight;
uniform float nearPlane;
uniform vec3 eyeUp;
uniform vec3 bbMin;
uniform vec3 bbMax;
uniform vec3 screenSize;

//Ray states (stored on rayDir.a)
#define INACTIVE 0.0
#define ACTIVE_TRAVERSE 1.0
#define ACTIVE_INTERSECT 2.0
#define OVERFLOW 3.0

//Ray voxel (stored on rayPos.a)

vec3 getRay(vec2 screenPos){

	vec3 u = (screenPos.x/(screenSize.x - screenSize.x/2.0)) * eyeRight;
	vec3 v = (screenPos.y/(screenSize.y - screenSize.y/2.0)) * eyeUp;
	vec3 point = (eyePos + nearPlane*eyeDir) + u + v;

	return normalize(point - eyePos);
}

bool insideGrid(vec3 position){

	if(position.x >= bbMin.x && position.y >= bbMin.y && position.z >= bbMin.z
		&& position.x <= bbMax.x && position.y <= bbMax.y && position.z <= bbMax.z)
		return true;
	else
		return false;

}

bool hitGrid(vec3 position, vec3 direction, out vec3 intersectionMin, out vec3 intersectionMax){

	intersectionMin = vec3(0.0, 0.0, 0.0);
	intersectionMax = vec3(0.0, 0.0, 0.0);
	if(direction.x >= 0.0){
		intersectionMin.x = (bbMin.x - position.x) / direction.x;
		intersectionMax.x = (bbMax.x - position.x) / direction.x;
	}
	else{
		intersectionMax.x = (bbMin.x - position.x) / direction.x;
		intersectionMin.x = (bbMax.x - position.x) / direction.x;
	}

	if(direction.y >= 0.0){
		intersectionMin.y = (bbMin.y - position.y) / direction.y;
		intersectionMax.y = (bbMax.y - position.y) / direction.y;
	}
	else{
		intersectionMax.y = (bbMin.y - position.y) / direction.y;
		intersectionMin.y = (bbMax.y - position.y) / direction.y;
	}

	if(intersectionMin.x > intersectionMax.y || intersectionMin.y > intersectionMax.x) return false;

	if(direction.z >= 0.0){
		intersectionMin.z = (bbMin.z - position.z) / direction.z;
		intersectionMax.z = (bbMax.z - position.z) / direction.z;
	}
	else{
		intersectionMax.z = (bbMin.z - position.z) / direction.z;
		intersectionMin.z = (bbMax.z - position.z) / direction.z;
	}

	if(intersectionMin.x > intersectionMax.z || intersectionMin.z > intersectionMax.x) return false;

	return true;
}

void main(){

	vec4 rayDir = vec4(getRay(gl_TexCoord[0].xy), 1.0);
	vec3 rayPos = eyePos;


	vec3 intersectionMin = vec3(0, 0, 0);
	vec3 intersectionMax = vec3(0, 0, 0);
	//Sets the ray state
	//The camera is inside the grid
	float breakpoint = 0.0;
	if(insideGrid(rayPos)){
		rayDir.a = ACTIVE_TRAVERSE;
		rayPos = rayPos;
		
	}
	else{
		//Check if the ray hits the grid
		if(hitGrid(rayPos, rayDir.xyz, intersectionMin, intersectionMax)){
			rayDir.a = ACTIVE_TRAVERSE;
			rayPos = intersectionMin;
			breakpoint = 1.0;
		}
		else{
			rayDir.a = INACTIVE;
		}
	}

	gl_FragData[0] = vec4(vec3(breakpoint), 1.0);
	/*
	gl_FragData[0] = vec4(rayPos, -1.0);
	gl_FragData[1] = rayDir;
	gl_FragData[2] = vec4(intersectionMin, 0.0);
	gl_FragData[3] = vec4(intersectionMax, 0.0);
	*/
	/*
	gl_FragData[0] = vec4(1.0, 1.0, 1.0, 1.0);
	gl_FragData[1] = vec4(0, 1, 0, 1.0);
	gl_FragData[2] = vec4(0, 0, 1, 1);
	gl_FragData[3] = vec4(0, 1, 1, 1);
	*/

}
