#version 330

layout (location = 0) in  vec4 position;

out vec3 TexCoords;

uniform mat4 proj;
uniform mat4 model;

void main() {
	TexCoords = vec3(position);
	gl_Position =  proj * model * position;
}