#version 330 core\n

in vec2 TexCoords;\n

out vec4 FragColor;\n

uniform sampler2D screenTexture;\n

//const float offset = 1.0f / 300.0f;\n//For kernel

void main(){\n
	FragColor = texture(screenTexture,TexCoords);\n

	//Inverse
	//FragColor = vec4(vec3(1.0 - texture(screenTexture,TexCoords)),1.0);\n

	//Grayscale
	//FragColor = texture(screenTexture,TexCoords);\n
	//Can generate good results but the human eye is more sensitive to green and the least to blue
	//float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;\n
	//float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;\n
	//FragColor = vec4(average,average,average,1.0);\n

	//Kernel
	//Assuming 3x3 kernel(which most kernels are)
	/*vec2 offsets[9] = vec2[](\n
		vec2(-offset, offset),\n//Top-left
		vec2(0.0f, offset),\n//Top-center
		vec2(offset, offset),\n//Top-right
		vec2(-offset, 0.0f),\n//Center-left
		vec2(0.0f, 0.0f),\n//Center-center
		vec2(offset, 0.0f),\n//Center-right
		vec2(-offset, -offset),\n//Bottom-left
		vec2(0.0f, -offset),\n//Bottom-center
		vec2(offset, -offset)\n//Bottom-right
	);\n

	//Sharpen kernel - sharpens each color by sampling all surronding pixels
	float kernel[9] = float[](\n
		-1,-1,-1,\n
		-1,9,-1,\n
		-1,-1,-1\n
	);\n

	//Blur - can also be used to smooth color values
	float blur[9] = float[](\n
		1.0f/16f, 2.0f/16f, 1.0f/16f,\n
		2.0f/16f, 4.0f/16f, 2.0f/16f,\n
		1.0f/16f, 2.0f/16f, 1.0f/16f\n
	);\n

	//Edge detection
	float edge[9] = float[](\n
		1,1,1,\n
		1,-8,1,\n
		1,1,1\n
	);\n

	vec3 sampleTex[9];\n
	for(int i = 0; i < 9; i++){\n
		sampleTex[i] = vec3(texture(screenTexture,TexCoords + offsets[i]));\n
	}\n

	vec3 col = vec3(0.0);\n
	for(int i = 0; i < 9; i++){\n
		col += sampleTex[i] * kernel[i];\n
	}\n

	FragColor = vec4(col,1.0);\n*/
}