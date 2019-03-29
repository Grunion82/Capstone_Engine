#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <sstream>

#include "Mesh.h"

class ModelLoader {
private:
	std::vector<glm::vec3> verticies;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;

	std::vector<GLuint> indicies;
	std::vector<GLuint> normalIndicies;
	std::vector<GLuint> textureIndicies;

	std::vector<Vertex> meshVerticies;

	std::vector<SubMesh> meshes;

	void PostProcessing();

public:
	glm::vec3 minV;
	glm::vec3 maxV;

	ModelLoader();
	~ModelLoader();

	void LoadModel(const std::string& filename);
	void LoadModel(const std::string& filename, const std::string& matname);

	std::vector<Vertex> GetVerts();
	std::vector<GLuint> GetIndicies();
	std::vector<SubMesh> GetMeshes();
};

#endif // ! MODELLOADER_H