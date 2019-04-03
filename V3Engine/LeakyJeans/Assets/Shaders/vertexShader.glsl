#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 Normal;
out vec2 TexCoords;
out vec3 FragPosition;
out vec3 eyeVec;
out vec3 lightDir;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec3 lightPos;

void main() {

	gl_Position = proj * view * model * vec4(position, 1.0f);
	Normal = mat3(transpose(inverse(model))) * normal;
	TexCoords = texCoords;
	FragPosition = vec3(model * vec4(position, 1.0f));
	vec4 eyePos = view * model * vec4(position, 1.0f);
	eyeVec = eyePos.xyz;
	lightDir = normalize(lightPos - eyeVec);
}