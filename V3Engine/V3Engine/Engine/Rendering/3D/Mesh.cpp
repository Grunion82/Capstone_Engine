#include "Mesh.h"

#include "../../Core/Systems/Camera.h"
#include "../Shader/Shader.h"

Mesh::Mesh(SubMesh subMesh_) : VAO(0), VBO(0) {
	VAO = 0;
	VBO = 0;
	subMesh = subMesh_;
	GenerateBuffers();
}


Mesh::~Mesh() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	delete meshShader;
	meshShader = nullptr;
}

void Mesh::GenerateBuffers() {
	//GENERATION VAO
	glGenVertexArrays(1, &VAO);
	//GENERATING vbo
	glGenBuffers(1, &VBO);

	/********************/
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(
		GL_ARRAY_BUFFER,//Type of buffer
		subMesh.vertexList.size() * sizeof(Vertex), // Size of the array in bytes
		&subMesh.vertexList[0],// Addres of the start of teh arrray 
		GL_STATIC_DRAW);//draw type

	//Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(

		0,//vertex know the location
		3,//how many objects in this attribute
		GL_FLOAT,//type of teh objects
		GL_FALSE,//if data is normalized then true or false
		sizeof(Vertex),// location + sizeof vec
		(GLvoid*)0);//stride place to look for teh first vertex of the array /**MESS WITH THIS**/

	//NORMALS
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(

		1,//vertex know the location
		3,//how many objects in this attribute
		GL_FLOAT,//type of teh objects
		GL_FALSE,//if data is normalized then true or false
		sizeof(Vertex),// location + sizeof vec
		(GLvoid*)offsetof(Vertex, normal));//stride place to look for teh first vertex of the array /**MESS WITH THIS**/

	//TEX-COORDS
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(

		2,//vertex know the location
		2,//how many objects in this attribute
		GL_FLOAT,//type of teh objects
		GL_FALSE,//if data is normalized then true or false
		sizeof(Vertex),// location + sizeof vec
		(GLvoid*)offsetof(Vertex, texCoords));//stride place to look for teh first vertex of the array /**MESS WITH THIS**/



	//unbind stuff
	glBindVertexArray(0); //WHAT HAPPENS WHEN U BIND? = zero to break the existing vertex array object binding.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*lightPosLoc = glGetUniformLocation(shaderProgram, "light.lightPos");
	lightAmbientLoc = glGetUniformLocation(shaderProgram, "light.ambient");
	lightDiffuseLoc = glGetUniformLocation(shaderProgram, "light.diffuse");
	lightColourLoc = glGetUniformLocation(shaderProgram, "light.lightColor");

	diffuseMapLoc = glGetUniformLocation(shaderProgram, "material.diffuseMap");
	shininessLoc = glGetUniformLocation(shaderProgram, "material.shininess");

	transparencyLoc = glGetUniformLocation(shaderProgram, "material.transparency");
	ambientLoc = glGetUniformLocation(shaderProgram, "material.ambient");
	diffuseLoc = glGetUniformLocation(shaderProgram, "material.diffuse");
	specularLoc = glGetUniformLocation(shaderProgram, "material.specular");*/
}

void Mesh::Render(const Camera* camera, const glm::mat4 modelMatrix) {
	/*glUniform1i(diffuseMapLoc, 0);//tells the shader we using the first texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, subMesh.material.diffuseMap);

	glUniform1f(shininessLoc, subMesh.material.shininess);
	glUniform1f(transparencyLoc, subMesh.material.transparency);
	glUniform3f(ambientLoc, subMesh.material.ambient.x, subMesh.material.ambient.y, subMesh.material.ambient.z);
	glUniform3f(diffuseLoc, subMesh.material.diffuse.x, subMesh.material.diffuse.y, subMesh.material.diffuse.z);
	glUniform3f(specularLoc, subMesh.material.specular.x, subMesh.material.specular.y, subMesh.material.specular.z);

	//light
	glUniform3f(viewPositionLoc, camera_->GetPosition().x, camera_->GetPosition().y, camera_->GetPosition().z);
	glUniform3f(lightPosLoc, camera_->GetLightSources()[0]->GetPosition().x, camera_->GetLightSources()[0]->GetPosition().y, camera_->GetLightSources()[0]->GetPosition().z);
	glUniform1f(lightAmbientLoc, camera_->GetLightSources()[0]->GetAmbient());
	glUniform1f(lightDiffuseLoc, camera_->GetLightSources()[0]->GetDiffuse());
	glUniform3f(lightColourLoc, camera_->GetLightSources()[0]->GetLightColour().x, camera_->GetLightSources()[0]->GetLightColour().y, camera_->GetLightSources()[0]->GetLightColour().z);

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera_->GetView()));
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(camera_->GetPerspective()));


	for (int i = 0; i < instances_.size(); i++) {

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(instances_[i]));

		//glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexList.size());
	}*/

	glBindVertexArray(VAO);

	meshShader->SetMat4("model", modelMatrix);
	meshShader->SetVec3("eyePos", camera->GetPosition());
	meshShader->SetMat4("view", camera->GetViewMatrix());
	meshShader->SetMat4("proj", camera->GetProjectionMatrix());
	meshShader->SetVec3("lightPos", glm::vec3(0.0f, 75.0f, 25.0f));

	glDrawArrays(GL_TRIANGLES, 0, subMesh.vertexList.size());

	glBindVertexArray(0);


}
