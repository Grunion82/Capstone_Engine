#ifndef MODEL_H
#define MODEL_H

#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "ModelLoader.h"
#include "../Shader/Shader.h"
#include "../2D/Image.h"

class Model {
private:
	std::vector<Mesh*> subMeshes;	// Vector of Mesh objects
	Shader* shaderProgram;			//The shader the model will use
	Texture* textureMap;			//The texture map the model will use

	//Loads the model
	void LoadModel();

	//Model's Transform-- Will get from referenceObject
	glm::mat4 transform;

	//Min and Max verts for easy Box Collider creation
	glm::vec3 MinVert;
	glm::vec3 MaxVert;

	//Object that the model is attached to
	class GameObject* referenceObject;

	//Object to Load the Model
	ModelLoader* modelLoader;

public:
	//Constructor and Destructor
	Model(class GameObject* refObj, const std::string& objPath_);
	~Model();

	//Updates the Model
	virtual void Update(float deltaTime);

	//Renders the Model
	virtual void Render(const class Camera* camera);

	//Add a mesh to the vector
	void AddMesh(Mesh* mesh);

	//Assigns the Shader program
	void SetShader(Shader* newShader);

	//Assigns the Texture map
	void SetTextureMap(Texture* newMap);

	//Retrieves the model transform
	inline glm::mat4 GetTransform() const { return transform; }
	//Retrieves the Shader program
	inline Shader* GetShader() const { return shaderProgram; }
	//Retrieves the Texture map
	inline Texture* GetTextureMap() const { return textureMap; }
	//Retrieves the MinVert of the Model
	inline glm::vec3 GetMinVert() const { return MinVert; }
	//Retrieves the MaxVert of the Model
	inline glm::vec3 GetMaxVert() const { return MaxVert; }
};
#endif // ! MODEL_H