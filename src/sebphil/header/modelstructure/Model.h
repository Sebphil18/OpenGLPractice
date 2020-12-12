#pragma once
#include "Mesh.h"
#include "image/Image.h"
#include "image/ImageLoader.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <vector>
#include <string>
#include <unordered_map>

class Model {

	// TODO: When texture could not be loaded: texture will not be added to mesh -> next valid Texture for mesh becomes the first.

private:

	struct Transformations {
		glm::vec3 translation;
		glm::vec3 size;
		glm::vec3 rotation;
	};

	std::vector<Mesh> meshes;
	std::unordered_map<const char*, TextureContainer> loadedTextures;

	Transformations transforms;

	void initializeTransforms();

	void destroyMeshes();

	void deleteLoadedTextures();
	void deleteTexture(unsigned int textureID);

	bool textureIsLoaded(const char* filePath);

	void loadTexImg(const char* filePath, TextureType textureType, Mesh& mesh);
	TextureContainer&& getTexImgContainer(const ImageLoader& imgLoader, TextureType textureType);
	void addTextureToMesh(TextureContainer texture, Mesh& mesh);
	unsigned int generateTexImg(const ImageLoader& imgLoader, TextureType textureType);
	unsigned int generateTexture();
	void bindTexture(unsigned int textureID);
	void setTexImgData(const ImageLoader& imgLoader, TextureType textureType);
	void generateMipmap();
	void setTexParameter();

	glm::mat4 getRotationMatrix() const;

public:

	Model();
	~Model();

	void draw(ShaderProgram& program);

	void addMesh(const Mesh& mesh);
	void pushbackNewMesh();
	void setMesh(Mesh mesh, std::size_t meshIndex);
	void removeMesh(std::size_t index);

	Mesh& getMesh(std::size_t meshIndex);
	Mesh& getLastMesh();

	void addTexture2D(const char* filePath, TextureType textureType, std::size_t meshIndex);
	void removeTexture(const char* filePath, std::size_t meshIndex);

	std::size_t getMeshCount() const;

	glm::mat4 getWorldMat() const;
	glm::mat4 getNormalMat() const;

	void setPosition(glm::vec3 position);
	void setSize(glm::vec3 size);
	void setRotation(glm::vec3 rotation);

	const glm::vec3& getPosition() const;
	const glm::vec3& getSize() const;
	const glm::vec3& getRotation() const;

};