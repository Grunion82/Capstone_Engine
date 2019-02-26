#version 330
layout (location = 0) in  vec3 aPos;
layout (location = 1) in  vec3 aNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	//mat3 normalMatrix = mat3(transpose(inverse(model)));
	//normal = normalMatrix * aNormal;

	gl_Position =  projection * view * model * vec4(aPos,1.0); 
}