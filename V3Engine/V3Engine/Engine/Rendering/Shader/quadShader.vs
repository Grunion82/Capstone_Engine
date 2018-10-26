#version 330 core

//Quad shader to render scene onto a 2D quad
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main(){
	gl_Position = vec4(aPos,1.0);
	TexCoords = aTexCoords;
}