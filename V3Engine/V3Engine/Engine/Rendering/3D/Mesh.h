#ifndef MESH_H
#define MESH_H

#include <glew.h>
#include <vector>
#include <glm/glm.hpp>//esto lo necesito para mi sistema de particulas;
#include <glm/gtc/type_ptr.hpp>//
#include <glm/gtx/string_cast.hpp>

#include "../../Core/Systems/Debug.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct SubMesh {
	std::vector<Vertex> vertexList;
	std::vector<GLuint> indices;
};

class Mesh {
private:
	GLuint VAO, VBO;
	SubMesh subMesh;
	void GenerateBuffers();
	class Shader* meshShader;
	GLuint modelLocation, viewLocation, projLocation, viewPositionLoc;
	//GLuint lightPosLoc, lightAmbientLoc, lightDiffuseLoc, lightColourLoc;
	//GLuint diffuseMapLoc, shininessLoc, transparencyLoc, ambientLoc, diffuseLoc, specularLoc;

public:
	Mesh(SubMesh subMesh_);
	~Mesh();
	void Render(const class Camera* camera, const glm::mat4 modelMatrix);

	inline void SetShader(Shader* shader) { meshShader = shader; }
};

#endif // !MESH_H