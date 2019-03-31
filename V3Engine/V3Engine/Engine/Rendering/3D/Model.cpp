#include "Model.h"

#include "../../Core/Game/GameObject.h"

Model::Model(GameObject* refObj, const std::string& objPath_) : subMeshes(std::vector<Mesh*>()) {
	referenceObject = refObj;
	transform = referenceObject->GetTransform().TransformationMatrix;
	modelLoader = new ModelLoader();
	modelLoader->LoadModel(objPath_);
	this->LoadModel();
}

Model::~Model() {
	if (subMeshes.size() > 0) {
		for (auto m : subMeshes) {

			delete m;
			m = nullptr;

		}

		subMeshes.clear();
		subMeshes.shrink_to_fit();
	}

	//delete shaderProgram;
	//shaderProgram = nullptr;

	delete textureMap;
	textureMap = nullptr;

	referenceObject = nullptr;

}

void Model::Update(float deltaTime) {
	transform = referenceObject->GetTransform().TransformationMatrix;
}


void Model::Render(const Camera* camera_) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureMap->ID());
	shaderProgram->Use();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (auto m : subMeshes) {
		m->Render(camera_, transform);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Model::AddMesh(Mesh* mesh_) {

	subMeshes.push_back(mesh_);

}

void Model::SetShader(Shader* newShader) {
	shaderProgram = newShader;
	for (Mesh* m : subMeshes) {
		m->SetShader(newShader);
	}
}

void Model::SetTextureMap(Texture* newMap) {
	textureMap = newMap;
}

void Model::LoadModel() {
	for (int i = 0; i < modelLoader->GetMeshes().size(); ++i) {
		subMeshes.push_back(new Mesh(modelLoader->GetMeshes()[i]));
	}

	MinVert = modelLoader->minV;
	MaxVert = modelLoader->maxV;

	//Delete object and return memory when finished using
	delete modelLoader;
	modelLoader = nullptr;
}