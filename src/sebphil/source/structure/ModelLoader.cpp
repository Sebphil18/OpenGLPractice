#pragma once
#include "structure/ModelLoader.h"
#include "glm/glm.hpp"
#include <iostream>
#include <vector>

ModelLoader::ModelLoader() {
	scene = nullptr;
}

ModelLoader::ModelLoader(const std::string& filePath) {
	loadModel(filePath.c_str());
}

void ModelLoader::loadModel(const std::string& filePath){

	std::size_t pathEnd = filePath.find_last_of('/');
	directory = filePath.substr(0, pathEnd);

	scene = importModelFile(filePath.c_str());

	if (sceneIsNotValid()) {
		std::cout << "ERROR::MODELLOADER::Could not load model " << filePath << "!" << std::endl;
		return;
	}

	examineNode(scene->mRootNode);
}

const aiScene* ModelLoader::importModelFile(const char* filePath) {
	return importer.ReadFile(filePath,
		aiProcess_Triangulate | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes | aiProcess_JoinIdenticalVertices 
		| aiProcess_GenNormals | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace);
}

bool ModelLoader::sceneIsNotValid() {
	return !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode;
}

void ModelLoader::examineNode(aiNode* node) {
	loadMeshes(node);
	expandNode(node);
}

void ModelLoader::loadMeshes(aiNode* node) {

	for (size_t i = 0; i < node->mNumMeshes; i++) {

		aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
		addAiMesh(aiMesh);
	}

}

void ModelLoader::addAiMesh(aiMesh* aiMesh) {

	pushbackNewMesh();
	Mesh& mesh = getLastMesh();
	
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<TextureContainer> textures;

	loadVertices(vertices, aiMesh);
	loadIndices(indices, aiMesh);

	Material material = getMeshMaterial(aiMesh);
	mesh.setMaterial(material);

	mesh.setData(vertices, indices);

}

void ModelLoader::loadVertices(std::vector<Vertex>& vertices, aiMesh* mesh) {

	for (size_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++)
		addVertex(vertices, mesh, vertexIndex);

}

void ModelLoader::addVertex(std::vector<Vertex>& vertices, aiMesh* mesh, std::size_t vertexIndex) {

	Vertex vertex;
	vertex.position = getVertexPosition(mesh, vertexIndex);
	vertex.normal = getVertexNormal(mesh, vertexIndex);
	vertex.texCoord = getVertexTexCoord(mesh, vertexIndex);
	vertex.tangent = getVertexTagent(mesh, vertexIndex);
	vertex.bitangent = getVertexBitagent(mesh, vertexIndex);

	vertices.push_back(vertex);
}

glm::vec3 ModelLoader::getVertexPosition(aiMesh* mesh, std::size_t vertexIndex) {
	aiVector3D position = mesh->mVertices[vertexIndex];
	return convertAiVec3dToGlmVec(position);
}

glm::vec3 ModelLoader::getVertexNormal(aiMesh* mesh, std::size_t vertexIndex) {
	aiVector3D normal = mesh->mNormals[vertexIndex];
	return convertAiVec3dToGlmVec(normal);
}

glm::vec2 ModelLoader::getVertexTexCoord(aiMesh* mesh, std::size_t vertexIndex) {
	
	glm::vec2 texCoord(0);

	if (mesh->mTextureCoords[0]) {
		aiVector3D texCoordVec = mesh->mTextureCoords[0][vertexIndex];
		texCoord = convertAiVec3dToGlmVec(texCoordVec);
	}

	return texCoord;
}

glm::vec3 ModelLoader::getVertexTagent(aiMesh* mesh, std::size_t vertexIndex) {
	aiVector3D tangent(0, 0, 0);
	if (mesh->HasTangentsAndBitangents())
		tangent = mesh->mTangents[vertexIndex];
	return convertAiVec3dToGlmVec(tangent);
}

glm::vec3 ModelLoader::getVertexBitagent(aiMesh* mesh, std::size_t vertexIndex) {
	aiVector3D bitangent(0, 0, 0);
	if (mesh->HasTangentsAndBitangents())
		bitangent = mesh->mBitangents[vertexIndex];
	return convertAiVec3dToGlmVec(bitangent);
}

glm::vec3 ModelLoader::convertAiVec3dToGlmVec(aiVector3D vec) {
	return glm::vec3(vec.x, vec.y, vec.z);
}

void ModelLoader::loadIndices(std::vector<unsigned int>& indices, aiMesh* mesh) {

	for (size_t i = 0; i < mesh->mNumFaces; i++) {

		aiFace face = mesh->mFaces[i];

		for (size_t j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);

	}

}

Material ModelLoader::getMeshMaterial(aiMesh* mesh) {

	Material material;
	unsigned int materialIndex = mesh->mMaterialIndex;

	if (materialIndex >= 0) {

		aiMaterial* aiMaterial = scene->mMaterials[materialIndex];

		transferAiMatProperties(aiMaterial, material);

		loadMaterialTextures(aiMaterial);

	}

	return material;
}

void ModelLoader::transferAiMatProperties(aiMaterial* aiMaterial, Material& material) {
	material.diffuseColor = getMatDiffuseColor(aiMaterial);
	material.specularColor = getMatSpecularColor(aiMaterial);
	material.ambientColor = getMatAmbientColor(aiMaterial);
	material.shininess = getMatShininess(aiMaterial);
}

glm::vec4 ModelLoader::getMatDiffuseColor(aiMaterial* aiMaterial) {

	aiColor4D diffuseColor;
	aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);

	return convertAiColor4dToGlmVec(diffuseColor);
}

glm::vec4 ModelLoader::getMatSpecularColor(aiMaterial* aiMaterial) {

	aiColor4D specularColor;
	aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);

	return convertAiColor4dToGlmVec(specularColor);
}

glm::vec4 ModelLoader::getMatAmbientColor(aiMaterial* aiMaterial) {

	aiColor4D ambientColor;
	aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);

	return convertAiColor4dToGlmVec(ambientColor);
}

glm::vec4 ModelLoader::convertAiColor4dToGlmVec(aiColor4D color) {
	return glm::vec4(color.r, color.g, color.b, color.a);
}

float ModelLoader::getMatShininess(aiMaterial* aiMaterial) {

	float shininess;
	aiMaterial->Get(AI_MATKEY_SHININESS, shininess);

	return shininess;
}

void ModelLoader::loadMaterialTextures(aiMaterial* aiMaterial) {
	loadTextures(aiMaterial, aiTextureType::aiTextureType_DIFFUSE);
	loadTextures(aiMaterial, aiTextureType::aiTextureType_SPECULAR);
	loadTextures(aiMaterial, aiTextureType::aiTextureType_AMBIENT);
	loadTextures(aiMaterial, aiTextureType::aiTextureType_NORMALS);
}

void ModelLoader::loadTextures(aiMaterial* material, aiTextureType aiTextureType) {

	std::vector<TextureContainer> textures;
	TextureType textureType = convertAiTexType(aiTextureType);

	for (size_t i = 0; i < material->GetTextureCount(aiTextureType); i++) {

		aiString file;
		material->GetTexture(aiTextureType, i, &file);

		std::string filePath = directory + '/' + file.C_Str();

		// Warning: May cause bug when this function is run parallel -> wrong mesh could be referenced! (new meshes could be added between pushbackNewMesh() and getMeshCount())
		// Can be fixed by using meshIndex as function argument.
		addTexture2D(filePath.c_str(), textureType, getMeshCount() - 1);
	}

}

TextureType ModelLoader::convertAiTexType(aiTextureType aiTextureType) {

	TextureType textureType = TextureType::none;

	switch (aiTextureType) {

	case aiTextureType::aiTextureType_DIFFUSE:
		textureType = TextureType::diffuse;
		break;

	case aiTextureType::aiTextureType_SPECULAR:
		textureType = TextureType::specular;
		break;

	case aiTextureType::aiTextureType_AMBIENT:
		textureType = TextureType::ambient;
		break;
	case aiTextureType::aiTextureType_NORMALS:
		textureType = TextureType::normal;
		break;
	default:
		textureType = TextureType::none;
		break;
	}

	return textureType;
}

void ModelLoader::expandNode(aiNode* node) {
	for (size_t i = 0; i < node->mNumChildren; i++)
		examineNode(node->mChildren[i]);
}