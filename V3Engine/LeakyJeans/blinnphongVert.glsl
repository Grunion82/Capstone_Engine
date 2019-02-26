#version 330
layout (location = 0) in  vec3 aPos;
layout (location = 1) in  vec3 aNormal;

out vec3 normal;
out vec3 lightDir;
out vec3 viewDir;
 
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 lightPos;

void main() {
	mat3 normalMatrix = mat3(transpose(inverse(model)));
	normal = normalMatrix * aNormal;
	vec4 eyePos = view * model * vec4(aPos,1.0); /// Create the eye vector 
	lightDir = normalize(lightPos - eyePos.xyz); /// Create the light direction 
	viewDir = normalize(aPos - eyePos.xyz);

	gl_Position =  projection * view * model * vec4(aPos,1.0); 
}
