#version 330
layout(location = 0) in  vec3 vVertex;
layout(location = 1) in  vec3 vNormal;

out vec3 vertNormal;
out vec3 lightDir;
out vec3 viewDir;
out vec3 eyeVec;
 
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 lightPos;

void main() {
	mat3 normalMatrix = mat3(transpose(inverse(model)));
	vertNormal = normalMatrix * vNormal;/// Rotate the normal to the correct orientation
	vec4 eyePos = view * model * vec4(vVertex,1.0); /// Create the eye vector 
	eyeVec = eyePos.xyz;
	lightDir = normalize(lightPos - eyeVec); /// Create the light direction 
	viewDir = normalize(vVertex - eyePos.xyz);

	gl_Position = projection * view * model * vec4(vVertex,1.0); 
}
