#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

//Preprocessor directitves - prevent linking conflicts
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	//program ID
	unsigned int ID;

	//Read and build the shader
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	//Activate shader
	void Use();
	//Utility uniform functions
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
};

#endif // !SHADER_H

