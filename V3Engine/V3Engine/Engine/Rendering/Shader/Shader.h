#ifndef SHADER_H
#define SHADER_H

#include <glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath);
	Shader(std::string& vertexPath, std::string& fragmentPath);
	Shader(std::string& vertexPath, std::string& fragmentPath,std::string& geometryPath);
	~Shader();

	//Activate shader
	void Use();
	//Utility uniform functions
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;

	//Vec
	void SetVec2(const char* name, float x,float y);
	void SetVec2(const char* name, glm::vec2 vec);
	void SetVec3(const char* name, float x,float y,float z);
	void SetVec3(const char* name, glm::vec3 vec);
	void SetVec4(const char* name, float x, float y, float z,float w);
	void SetVec4(const char* name, glm::vec4 vec);

	//Matrix
	void SetMat4(const char* name, glm::mat4 newMat);
};

#endif // !SHADER_H

