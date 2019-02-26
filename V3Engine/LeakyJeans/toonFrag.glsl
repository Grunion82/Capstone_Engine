#version 330

in  vec3 vertNormal;
in  vec3 lightDir;
in vec3 viewDir;
in  vec3 eyeVec;

out vec4 fragColor;

void main() {

	//float intensity = dot(lightDir, vertNormal);
	vec3 halfway = normalize(lightDir + viewDir);
	
	float intensity = dot(lightDir, halfway);
	
	vec4 color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	float silhouette = max(dot(vertNormal,normalize(-eyeVec)), 0.0f);

	//Border
	if (silhouette < 0.2f){
		fragColor = color;	
	}
	//White
	else if (intensity > 0.95f){
		fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (intensity > 0.5f){
		fragColor = vec4(0.6f, 0.6f, 0.6f, 1.0f);
	}
	else if (intensity > 0.25f){
		fragColor = vec4(0.4f, 0.4f, 0.4f, 1.0f);
	}
	else {
		fragColor = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	}	
} 

