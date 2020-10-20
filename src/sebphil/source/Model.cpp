#include "header/Model.h"
#include "stbi/stb_Image.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include <iostream>

Model::Model() {
	initializeTransforms();
}

void Model::initializeTransforms() {
	glm::vec3 nullVec(0, 0, 0);
	glm::vec3 oneVec(1, 1, 1);
	transforms = { nullVec, oneVec, nullVec };
}

Model::~Model() {
	deleteLoadedTextures();
	destroyMeshes();
}

void Model::deleteLoadedTextures() {

	std::unordered_map<const char*, TextureContainer>::const_iterator iterator;

	for (iterator = loadedTextures.begin(); iterator != loadedTextures.end(); iterator++) {
		TextureContainer tex = iterator->second;
		deleteTexture(tex.id);
	}

}

void Model::destroyMeshes() {
	for (size_t i = 0; i < meshes.size(); i++) {
		Mesh& mesh = meshes[i];
		mesh.destroyBuffer();
		mesh.deleteTextures();
	}
}

void Model::deleteTexture(unsigned int textureID) {

	glDeleteTextures(1, &textureID);

}

void Model::draw(ShaderProgram& program) {
	for (size_t i = 0; i < meshes.size(); i++)
		meshes[i].draw(program);
}

void Model::addTexture2D(const char* filePath, TextureType textureType, std::size_t meshIndex) {

	if (textureType == TextureType::none)
		return;

	Mesh& mesh = meshes[meshIndex];

	if (textureIsLoaded(filePath)) {

		TextureContainer& texture = loadedTextures[filePath];
		addTextureToMesh(texture, mesh);

	} else
		loadTexImg(filePath, textureType, mesh);

}

void Model::loadTexImg(const char* filePath, TextureType textureType, Mesh& mesh) {

	ImageLoader imgLoader;
	imgLoader.loadRGBA(filePath);

	TextureContainer texture = getTexImgContainer(imgLoader, textureType);

	addTextureToMesh(texture, mesh);
	loadedTextures[filePath] = texture;

}

TextureContainer&& Model::getTexImgContainer(const ImageLoader& imgLoader, TextureType textureType) {
	unsigned int textureID = generateTexImg(imgLoader, textureType);
	return { textureID, textureType };
}

unsigned int Model::generateTexImg(const ImageLoader& imgLoader, TextureType textureType) {

	unsigned int textureID = generateTexture();

	bindTexture(textureID);
	setTexImgData(imgLoader, textureType);
	generateMipmap();
	setTexParameter();

	return textureID;
}

unsigned int Model::generateTexture() {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	return textureID;
}

void Model::bindTexture(unsigned int textureID) {
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Model::setTexImgData(const ImageLoader& imgLoader, TextureType textureType) {
	glm::vec2 dimensions = imgLoader.getDimensions();

	if (textureType == TextureType::diffuse)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, dimensions.x, dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgLoader.getBuffer());
	else if(textureType != TextureType::none)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, dimensions.x, dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgLoader.getBuffer());

}

void Model::generateMipmap() {
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Model::setTexParameter() {

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void Model::addTextureToMesh(TextureContainer texture, Mesh& mesh) {
	if (!mesh.containsTexture(texture.id))
		mesh.addTexture(texture);
}

void Model::removeTexture(const char* filePath, std::size_t meshIndex) {

	if (textureIsLoaded(filePath)) {
		Mesh& mesh = meshes[meshIndex];
		TextureContainer& texture = loadedTextures[filePath];

		mesh.removeTexture(texture.id);
	}

}

bool Model::textureIsLoaded(const char* filePath) {
	return loadedTextures.find(filePath) != loadedTextures.end();
}

void Model::addMesh(const Mesh& mesh) {
	meshes.push_back(mesh);
}

void Model::setMesh(Mesh mesh, std::size_t meshIndex) {
	meshes[0] = mesh;
}

void Model::pushbackNewMesh() {
	meshes.push_back(Mesh());
}

void Model::removeMesh(std::size_t index) {
	meshes.erase(meshes.begin() + index);
}

Mesh& Model::getMesh(std::size_t meshIndex) {
	return meshes[meshIndex];
}

Mesh& Model::getLastMesh() {
	return meshes[meshes.size() - 1];
}

std::size_t Model::getMeshCount() const {
	return meshes.size();
}

glm::mat4 Model::getWorldMat() const {

	glm::mat4 worldMat(1);
	worldMat = glm::scale(worldMat, transforms.size);
	
	// TODO: glm::mat4 getRotationMat(glm::vec3 angles);
	glm::quat quaternion(transforms.rotation);
	glm::mat4 rotationMat = glm::toMat4(quaternion);
	worldMat *= rotationMat;

	worldMat = glm::translate(worldMat, transforms.translation);

	return worldMat;
}

glm::mat4 Model::getNormalMat() const {
	return glm::transpose(glm::inverse(getWorldMat()));
}

void Model::setPosition(glm::vec3 position) {
	transforms.translation = position;
}

void Model::setSize(glm::vec3 size) {
	transforms.size = size;
}

void Model::setRotation(glm::vec3 rotation) {
	transforms.rotation = rotation;
}

const glm::vec3& Model::getPosition() const {
	return transforms.translation;
}

const glm::vec3& Model::getSize() const {
	return transforms.size;
}

const glm::vec3& Model::getRotation() const {
	return transforms.rotation;
}