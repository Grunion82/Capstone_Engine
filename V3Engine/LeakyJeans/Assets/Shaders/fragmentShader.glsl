#version 330 core

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPosition;
in vec3 eyeVec;
in vec3 lightDir;

out vec4 color;

uniform sampler2D textureMap;
uniform vec3 viewPosition;

void main() {
	//diffuse
	vec4 kd = texture(textureMap, TexCoords);

	//specular 
	vec4 ks = vec4(1.0f, 1.0f, 1.0f, 0.0f);	

	//ambient
	vec4 ka = 0.1f * kd;	

	//Blinn-Phong Shading
	vec3 halfWayVec = normalize(lightDir + eyeVec);
	float diff = max(dot(Normal, lightDir), 0.0f);
	float spec = max(dot(Normal, halfWayVec), 0.0f);
	

	color = ka + (diff * kd) + (spec * ks);
}