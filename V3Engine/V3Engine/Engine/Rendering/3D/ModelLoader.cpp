#include "ModelLoader.h"


void ModelLoader::PostProcessing() {

	for (int i = 0; i < indicies.size(); ++i) {
		Vertex vert;
		vert.position = verticies[indicies[i] - 1];
		vert.normal = normals[normalIndicies[i] - 1];
		vert.texCoords = texCoords[textureIndicies[i] - 1];
		meshVerticies.push_back(vert);
	}
	SubMesh mesh;
	mesh.vertexList = meshVerticies;
	mesh.indices = indicies;
	meshes.push_back(mesh);

	indicies.clear();
	indicies.shrink_to_fit();

	normalIndicies.clear();
	normalIndicies.shrink_to_fit();

	textureIndicies.clear();
	textureIndicies.shrink_to_fit();

	meshVerticies.clear();
	meshVerticies.shrink_to_fit();

}

ModelLoader::ModelLoader() {
	minV = glm::vec3(0.0f);
	maxV = glm::vec3(0.0f);
}

ModelLoader::~ModelLoader() {

	verticies.clear();
	verticies.shrink_to_fit();

	normals.clear();
	normals.shrink_to_fit();

	texCoords.clear();
	texCoords.shrink_to_fit();

	indicies.clear();
	indicies.shrink_to_fit();

	normalIndicies.clear();
	normalIndicies.shrink_to_fit();

	textureIndicies.clear();
	textureIndicies.shrink_to_fit();

	meshVerticies.clear();
	meshVerticies.shrink_to_fit();

	meshes.clear();
	meshes.shrink_to_fit();
}

void ModelLoader::LoadModel(const std::string & filename) {

	std::ifstream in(filename, std::ios::in);
	if (!in) {
		Debug::Error("Cannot open .obj file: " + filename, __FILE__, __LINE__);
		return;
	}

	std::string line;
	double minX, minY, minZ;
	minX = minY = minZ = FLT_MAX;
	double maxX, maxY, maxZ;
	maxX = maxY = maxZ = FLT_MIN;

	

	while (std::getline(in, line)) {
		//VERTEX DATA
		if (line.substr(0, 2) == "v ") {
			std::istringstream v(line.substr(2));
			double x, y, z;
			v >> x >> y >> z;

			//Check for max and min X components

			if (x < minX) {
				minX = x;
			}
			if (x > maxX) {
				maxX = x;
			}

			//Check for max and min Y components

			if (y < minY) {
				minY = y;
			}
			if (y > maxY) {
				maxY = y;
			}

			//Check for max and min Z components

			if (z < minZ) {
				minZ = z;
			}
			else if (z > maxZ) {
				maxZ = z;
			}

			verticies.push_back(glm::vec3(x, y, z));
		}

		//NORMAL DATA
		else if (line.substr(0, 3) == "vn ") {
			std::istringstream vn(line.substr(3));
			double x, y, z;
			vn >> x >> y >> z;
			normals.push_back(glm::vec3(x, y, z));
		}

		//TEXTURE COORDS DATA
		else if (line.substr(0, 3) == "vt ") {
			std::istringstream vt(line.substr(3));
			double x, y, z;
			vt >> x >> y >> z;
			texCoords.push_back(glm::vec2(x, y));
		}

		//FACE DATA
		//(vertex/texture/nomral)
		//f 1/1/1 2/2/1 3/3/1
		else if (line.substr(0, 2) == "f ") {
			std::istringstream f(line.substr(2)); //1/1/1 2/2/1 3/3/1

			unsigned int i = 0;
			char c;

			f >> i;
			indicies.push_back(i);
			//Ignore slash
			f >> c;

			f >> i;
			textureIndicies.push_back(i);
			//Ignore slash
			f >> c;

			f >> i;
			normalIndicies.push_back(i);

			//Loop back for 2nd & 3rd sets of indicies
			f >> i;
			indicies.push_back(i);
			//Ignore slash
			f >> c;

			f >> i;
			textureIndicies.push_back(i);
			//Ignore slash
			f >> c;

			f >> i;
			normalIndicies.push_back(i);

			f >> i;
			indicies.push_back(i);
			//Ignore slash
			f >> c;

			f >> i;
			textureIndicies.push_back(i);
			//Ignore slash
			f >> c;

			f >> i;
			normalIndicies.push_back(i);
		}
	}

	minV = glm::vec3(minX, minY, minZ);
	maxV = glm::vec3(maxX, maxY, maxZ);

	PostProcessing();

}

void ModelLoader::LoadModel(const std::string & filename, const std::string & matname) {

	LoadModel(filename);
}

std::vector<Vertex> ModelLoader::GetVerts() {

	return meshVerticies;
}

std::vector<GLuint> ModelLoader::GetIndicies() {

	return indicies;
}

std::vector<SubMesh> ModelLoader::GetMeshes() {

	return meshes;
}