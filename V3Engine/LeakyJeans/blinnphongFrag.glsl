#version 330
in  vec3 normal;
in  vec3 lightDir;
in vec3 viewDir;

out vec4 fragColor;

void main() { 
	const vec4 ks = vec4(0.8, 0.8, 0.8, 0.0);
	const vec4 kd = vec4(0.2, 0.2, 0.8, 0.0);
	const vec4 ka = 0.1 * kd;					
		
	vec3 halfway = normalize(lightDir + viewDir);

	float diff = max(dot(normal,lightDir), 0.0);
	
	float spec = pow(max(dot(normal,halfway),0.0),16.0);
	
	fragColor =  ka + (diff * kd) + (spec * ks);
	
} 

