#pragma once
#include "header/Model.h"

class ModelLoader : public Model {

private:

	const aiScene* scene;
	std::string directory;
	Assimp::Importer importer;

	const aiScene* importModelFile(const char* filePath);
	bool sceneIsNotValid();
	void loadMeshes(aiNode* node);
	void expandNode(aiNode* node);
	void addAiMesh(aiMesh* aiMesh);

	void loadVertices(std::vector<Vertex>& vertices, aiMesh* mesh);
	void addVertex(std::vector<Vertex>& vertices, aiMesh* mesh, std::size_t vertexIndex);
	glm::vec3 getVertexPosition(aiMesh* mesh, std::size_t vertexIndex);
	glm::vec3 getVertexNormal(aiMesh* mesh, std::size_t vertexIndex);
	glm::vec2 getVertexTexCoord(aiMesh* mesh, std::size_t vertexIndex);
	glm::vec3 getVertexTagent(aiMesh* mesh, std::size_t vertexIndex);
	glm::vec3 getVertexBitagent(aiMesh* mesh, std::size_t vertexIndex);
	glm::vec3 convertAiVec3dToGlmVec(aiVector3D vec);

	void loadIndices(std::vector<unsigned int>& indices, aiMesh* mesh);

	Material getMeshMaterial(aiMesh* mesh);
	void transferAiMatProperties(aiMaterial* aiMaterial, Material& material);
	glm::vec4 getMatDiffuseColor(aiMaterial* aiMaterial);
	glm::vec4 getMatSpecularColor(aiMaterial* aiMaterial);
	glm::vec4 getMatAmbientColor(aiMaterial* aiMaterial);
	glm::vec4 convertAiColor4dToGlmVec(aiColor4D color);
	float getMatShininess(aiMaterial* aiMaterial);
	void loadMaterialTextures(aiMaterial* aiMaterial);
	void loadTextures(aiMaterial* material, aiTextureType aiTextureType);
	TextureType convertAiTexType(aiTextureType aiTextureType);

public:
	
	ModelLoader();
	ModelLoader(const std::string& filePath);
	
	void loadModel(const std::string& filePath);
	void examineNode(aiNode* node);

};