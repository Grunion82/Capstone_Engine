#ifndef SKYBOX_H
#define SKYBOX_H

#include "../../Rendering/3D/Mesh.h"
#include "../../Rendering/3D/ModelLoader.h"
#include "../../Rendering/Shader/Shader.h"
#include "../../Rendering/2D/Image.h"

class Skybox {
private:
	//Cube Mesh for the Skybox
	Mesh* cubeMesh;
	//6-sided Map texture
	Cubemap* cubeMap;
	//Corresponding Shader program
	Shader* shaderProgram;

	//For loading the mesh
	ModelLoader* modelLoader;

	//The Model/View matrix of the Skybox
	glm::mat4 identity;

	//Loads the box using the ModelLoader
	void LoadSkybox();

public:
	Skybox(std::vector<const char*> tempfaces, const char* filepath, Shader* newShader);
	~Skybox();

	//Standard Update and Render
	void Update(float deltaTime);
	void Render(const class Camera* camera);

	//Pass a vector of new faces
	inline void SetSkyboxMap(std::vector<const char*> faces) { cubeMap->SetFaces(faces); }
};


#endif // !SKYBOX_H
