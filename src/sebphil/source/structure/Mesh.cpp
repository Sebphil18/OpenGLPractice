#include "structure/Mesh.h"
#include "glad/glad.h"
#include <iostream>

Mesh::Mesh() {
	initializeBuffer();
	initializeMaterial();
	initializeLayout();
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<TextureContainer>& textures):

	vertices(vertices), indices(indices), textures(textures) {

	initializeBuffer();
	initializeMaterial();
	initializeLayout();

	fillBuffer();
}

void Mesh::initializeBuffer() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
}

void Mesh::initializeMaterial() {
	glm::vec4 diffuseColor(1, 1, 1, 1);
	glm::vec4 specularColor(1, 1, 1, 1);
	glm::vec4 ambientColor(0.1, 0.1, 0.1, 1);
	float shininess = 16;

	material = { diffuseColor, specularColor, ambientColor, shininess };
}

void Mesh::initializeLayout() {
	vboLayout.addElement({ 3, GL_FLOAT, GL_FALSE });
	vboLayout.addElement({ 3, GL_FLOAT, GL_FALSE });
	vboLayout.addElement({ 2, GL_FLOAT, GL_FALSE });
	vboLayout.addElement({ 3, GL_FLOAT, GL_FALSE }); 
	vboLayout.addElement({ 3, GL_FLOAT, GL_FALSE });
}

void Mesh::setData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<TextureContainer>& textures) {

	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	fillBuffer();

}

void Mesh::setData(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {

	this->vertices = vertices;
	this->indices = indices;

	fillBuffer();
}

void Mesh::fillBuffer() {
	bindBuffer();

	vboLayout.bindLayout();

	setVboData();
	setIboData();

	unbindBuffer();
}

void Mesh::updateVertices(const std::vector<Vertex>& vertices, std::vector<Vertex>::const_iterator begin, std::vector<Vertex>::const_iterator end) {

	std::size_t length = end - begin;
	uint32_t offset = (vertices.begin() - begin);
	uint32_t size = sizeof(Vertex) * length;

	updateBuffer(offset, size, &vertices[offset]);

	for (auto i = begin; i != end; i++) {
		uint32_t index = i - begin;
		this->vertices[index] = *i;
	}

}

void Mesh::updateBuffer(uint32_t offset, uint32_t size, const void* data) {
	bindBuffer();
	vboLayout.bindLayout();
	updateVboData(offset, size, data);
	unbindBuffer();
}

void Mesh::bindBuffer() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

void Mesh::setVboData() {
	if (!vertices.empty())
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
}

void Mesh::setIboData() {
	if (!indices.empty())
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

void Mesh::updateVboData(uint32_t offset, uint32_t size, const void* data) {
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void Mesh::unbindBuffer() {
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::destroyBuffer() {
	deleteBuffer();
}

void Mesh::deleteBuffer() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Mesh::deleteTextures() {
	for (TextureContainer texture : textures)
		glDeleteTextures(1, &texture.id);
	textures.clear();
}

void Mesh::draw(ShaderProgram& program) {
	activateTextures(program);
	loadMaterial(program);
	drawMesh(program);
	deactivateTextures();
}

void Mesh::activateTextures(ShaderProgram& program) {

	CountIterators iterators;

	for (size_t i = 0; i < textures.size(); i++) {

		TextureType type = textures[i].type;
		std::string textureName = getTexUniformName(iterators, type);

		program.setUniform1i(textureName.c_str(), i);

		bindTextureToSlot(i);
	}

}

std::string Mesh::getTexUniformName(CountIterators& iterators, TextureType texType) {

	std::string uniformName;

	if (texType == TextureType::diffuse) {
		uniformName = "material.diffuse" + std::to_string(iterators.diffuseCount);
		iterators.diffuseCount++;
	} else if (texType == TextureType::specular) {
		uniformName = "material.specular" + std::to_string(iterators.specularCount);
		iterators.specularCount++;
	} else if (texType == TextureType::ambient) {
		uniformName = "material.ambient" + std::to_string(iterators.ambientCount);
		iterators.ambientCount++;
	} else if (texType == TextureType::normal) {
		uniformName = "material.normal";
	}

	return uniformName;
}

void Mesh::bindTextureToSlot(unsigned int textureIndex) {
	glActiveTexture(GL_TEXTURE0 + textureIndex);
	glBindTexture(GL_TEXTURE_2D, textures[textureIndex].id);
}

void Mesh::loadMaterial(ShaderProgram& program) {
	program.setUniformVec4f("material.diffuseColor", material.diffuseColor);
	program.setUniformVec4f("material.specularColor", material.specularColor);
	program.setUniformVec4f("material.ambientColor", material.ambientColor);
	program.setUniform1f("material.shininess", material.shininess);
}

void Mesh::drawMesh(ShaderProgram& program) {

	program.use();
	
	glBindVertexArray(vao);

	if (!indices.empty())
		drawIndices();
	else if (!vertices.empty())
		drawVertices();

}

void Mesh::drawIndices() {	
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
}

void Mesh::drawVertices() {
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void Mesh::deactivateTextures() {
	for (size_t i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh::setMaterial(Material newMaterial) {
	material = newMaterial;
}

void Mesh::addTexture(const TextureContainer& texture) {
	textures.push_back(texture);
}

void Mesh::removeTexture(unsigned int textureID) {
	
	for (size_t i = 0; i < textures.size(); i++) {
		if (textureID == textures[i].id) {
			textures.erase(textures.begin() + i);
			return;
		}
	}

}

bool Mesh::containsTexture(unsigned int textureID) {

	for (TextureContainer tex : textures) {
		if (textureID == tex.id)
			return true;
	}

	return false;
}