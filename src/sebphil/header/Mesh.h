#pragma once
#include "glm/glm.hpp"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "ShaderProgram.h"
#include <vector>

enum class TextureType { none, diffuse, specular, ambient };

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
	};

	Material material;

	VertexBufferLayout vboLayout;

	unsigned int vbo, vao, ibo;

	void fillBuffer();

	void initializeBuffer();
	void initializeMaterial();
	void initializeLayout();

	void deleteBuffer();

	void bindBuffer();
	void setVboData();
	void setIboData();
	void unbindBuffer();

	void loadMaterial(ShaderProgram& program);

	void activateTextures(ShaderProgram& program);
	std::string getTexUniformName(CountIterators& iterators, TextureType texType);

	void bindTextureToSlot(unsigned int textureIndex);
	void drawMesh(ShaderProgram& program);
	void drawIndices();
	void drawVertices();

	void deactivateTextures();

	static Vertex getVertexAttributes(float vertices[], std::size_t arrIndex);

public:

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<TextureContainer> textures;

	static std::vector<Vertex> convertFloatToVertex(float vertices[], std::size_t size);

	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
		std::vector<TextureContainer> textures);

	void destroyBuffer();
	void deleteTextures();

	void setData(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
		std::vector<TextureContainer> textures);

	void setData(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

	void draw(ShaderProgram& program);
	void setMaterial(Material newMaterial);

	void addTexture(const TextureContainer& texture);
	void removeTexture(unsigned int textureID);

	bool containsTexture(unsigned int textureID);

};
