#version 330
//Multiple render targets - targets are based off the render target specified in graphics
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
//layout (location = 3) out vec2 gTexCoords;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main(){
	gPosition = FragPos;
	gNormal = normalize(Normal);
	gAlbedoSpec.rgb = texture(texture_diffuse1,TexCoords).rgb;//Diffuse
	gAlbedoSpec.a = texture(texture_specular1,TexCoords).r;//Specular
	//gTexCoords = vec3(TexCoords,0.0);
}