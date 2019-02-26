#version 330 core

in VS_OUT{
	vec3 fragPos;
	vec2 tex;
} fs_in;

out vec4 FragColor;

uniform sampler3D noiseVolume0;
uniform sampler3D noiseVolume1;
uniform sampler3D noiseVolume2;
uniform sampler3D noiseVolume3;



void main(){
	
	FragColor = vec4(0.2,1.0,0.2,1.0);
	
}