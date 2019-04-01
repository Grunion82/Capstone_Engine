#include "Skybox.h"

#include "../Systems/Camera.h"

void Skybox::LoadSkybox() {
	//GetMesh()[0] will always be the cube mesh. No other model should be loaded from here
	cubeMesh = new Mesh(modelLoader->GetMeshes()[0]);

	//Destroy the modelLoader object now that we're done with it
	delete modelLoader;
	modelLoader = nullptr;
}

Skybox::Skybox(std::vector<const char*> tempFaces, const char* filepath, Shader* newShader) {
	//Create object of cube map using textures
	cubeMap = new Cubemap(tempFaces);

	//Loads cube mesh
	modelLoader = new ModelLoader();
	modelLoader->LoadModel(filepath);
	LoadSkybox();

	//Create Skybox Shader program and assign to the mesh
	shaderProgram = newShader;
	//cubeMesh->SetShaderProgram(shaderProgram->ID);
	cubeMesh->SetShader(shaderProgram);

	//Set id
	identity = glm::mat4(1.0f);
}

Skybox::~Skybox() {
	//Clean up pointers
	delete cubeMesh;
	cubeMesh = nullptr;

	delete cubeMap;
	cubeMap = nullptr;

	//delete shaderProgram;
	//shaderProgram = nullptr;
}

void Skybox::Update(float deltaTime) {

}

void Skybox::Render(const Camera* camera) {
	//Disable depth testing and face culling for rendering specifically the Skybox
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//Use the Skybox's shader and texture
	glActiveTexture(GL_TEXTURE0);

	cubeMap->Use();
	shaderProgram->Use();

	cubeMesh->Render(camera, glm::mat4(glm::mat3(camera->GetViewMatrix()))); //mat4(mat3()) gets rid of the Translation from the ViewMatrix

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}