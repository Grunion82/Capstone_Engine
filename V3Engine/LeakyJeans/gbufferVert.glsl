#version 330 
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projeciton;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main(){	
	gl_Position = projeciton * view * model * vec4(aPosition,1.0f);
	TexCoords = aTexCoords;
	Normal = mat3(transpose(inverse(model))) * aNormal;
	FragPos = vec3(model * vec4(aPosition,1.0f));
}