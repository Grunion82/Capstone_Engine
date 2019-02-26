#version 330

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out VS_OUT{
	vec3 fragPos;
	vec2 tex;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	vs_out.fragPos = vec3(model * vec4(aPos.x,aPos.y,0.0,1.0));
	gl_Position = projection * view * model * vec4(aPos.x,aPos.y,0.0,1.0);
	vs_out.tex = aTexCoords;
}
