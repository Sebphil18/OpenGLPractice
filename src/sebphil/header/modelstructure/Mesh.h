#pragma once
#include "glm/glm.hpp"
#include "globjects/VertexBufferLayout.h"
#include "globjects/VertexBuffer.h"
#include "globjects/IndexBuffer.h"
#include "globjects/VertexArray.h"
#include "shader/ShaderProgram.h"
#include <vector>

enum class TextureType { none, diffuse, specular, ambient, normal};

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

struct Material {
	glm::vec4 diffuseColor;
	glm::vec4 specularColor;
	glm::vec4 ambientColor;
	float shininess;
};

struct TextureContainer {
	unsigned int id;
	TextureType type;
};

class Mesh {

private:
	struct CountIterators {
		unsigned int diffuseCount = 0;
		unsigned int specularCount = 0;
		unsigned int ambientCount = 0;
		unsigned int normalCount = 0;
	};

	Material material;

	VertexBufferLayout vboLayout;

	unsigned int vbo, vao, ibo;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<TextureContainer> textures;

	void initializeBuffer();
	void initializeMaterial();
	void initializeLayout();

	void deleteBuffer();

	void fillBuffer();
	void updateBuffer(uint32_t offset, uint32_t size, const void* data);

	void bindBuffer();
	void setVboData();
	void setIboData();
	void updateVboData(uint32_t offset, uint32_t size, const void* data);
	void unbindBuffer();

	void loadMaterial(ShaderProgram& program);

	void activateTextures(ShaderProgram& program);
	std::string getTexUniformName(CountIterators& iterators, TextureType texType);

	void bindTextureToSlot(unsigned int textureIndex);
	void drawMesh(ShaderProgram& program);
	void drawIndices();
	void drawVertices();

	void deactivateTextures();

public:
	Mesh();
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<TextureContainer>& textures);
	Mesh(std::size_t verticesLength);

	void destroyBuffer();
	void deleteTextures();

	void setData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
		const std::vector<TextureContainer>& textures);
	void setData(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	void updateVertices(const std::vector<Vertex>& vertices, std::vector<Vertex>::const_iterator begin, std::vector<Vertex>::const_iterator end);

	void draw(ShaderProgram& program);
	void setMaterial(Material newMaterial);

	void addTexture(const TextureContainer& texture);
	void removeTexture(unsigned int textureID);

	bool containsTexture(unsigned int textureID);

};
