#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D screenTexture;

//const float offset = 1.0f / 300.0f;//For kernel

void main(){
	FragColor = texture(screenTexture,TexCoords);
	
	//Inverse
	//FragColor = vec4(vec3(1.0 - texture(screenTexture,TexCoords)),1.0);

	//Grayscale
	//FragColor = texture(screenTexture,TexCoords);
	//Can generate good results but the human eye is more sensitive to green and the least to blue
	//float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
	//float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
	//FragColor = vec4(average,average,average,1.0);

	//Kernel
	//Assuming 3x3 kernel(which most kernels are)
	/*vec2 offsets[9] = vec2[](
		vec2(-offset, offset),//Top-left
		vec2(0.0f, offset),//Top-center
		vec2(offset, offset),//Top-right
		vec2(-offset, 0.0f),//Center-left
		vec2(0.0f, 0.0f),//Center-center
		vec2(offset, 0.0f),//Center-right
		vec2(-offset, -offset),//Bottom-left
		vec2(0.0f, -offset),//Bottom-center
		vec2(offset, -offset)//Bottom-right
	);

	//Sharpen kernel - sharpens each color by sampling all surronding pixels
	float kernel[9] = float[](
		-1,-1,-1,
		-1,9,-1,
		-1,-1,-1
	);

	//Blur - can also be used to smooth color values
	float blur[9] = float[](
		1.0f/16f, 2.0f/16f, 1.0f/16f,
		2.0f/16f, 4.0f/16f, 2.0f/16f,
		1.0f/16f, 2.0f/16f, 1.0f/16f
	);

	//Edge detection
	float edge[9] = float[](
		1,1,1,
		1,-8,1,
		1,1,1
	);

	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++){
		sampleTex[i] = vec3(texture(screenTexture,TexCoords + offsets[i]));
	}

	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++){
		col += sampleTex[i] * kernel[i];
	}

	FragColor = vec4(col,1.0);*/
}