#include "structure/VertexConverter.h"

std::vector<Vertex> VertexConverter::floatArrToVertexVec(float vertices[], std::size_t size) {
	std::vector<Vertex> convertedVertices;

	std::size_t length = size / sizeof(Vertex);
	std::size_t vertexAttributes = sizeof(Vertex) / sizeof(float);

	for (std::size_t i = 0; i < length; i++) {
		std::size_t arrIndex = i * vertexAttributes;
		Vertex vertex = getVertexAttributes(vertices, arrIndex);
		convertedVertices.push_back(vertex);
	}

	return convertedVertices;
}

Vertex VertexConverter::getVertexAttributes(float vertices[], std::size_t arrIndex) {
	glm::vec3 position = getVertexPosition(vertices, arrIndex);
	glm::vec3 normal = getVertexNormal(vertices, arrIndex);
	glm::vec2 texCoord = getVertexTexCoord(vertices, arrIndex);
	glm::vec3 tangent = getVertexTangent(vertices, arrIndex);
	glm::vec3 bitangent = getVertexBitangent(vertices, arrIndex);
	return { position, normal, texCoord, tangent, bitangent };
}

glm::vec3 VertexConverter::getVertexPosition(float vertices[], std::size_t arrIndex) {
	return glm::vec3(vertices[arrIndex], vertices[arrIndex + 1], vertices[arrIndex + 2]);
}

glm::vec3 VertexConverter::getVertexNormal(float vertices[], std::size_t arrIndex) {
	return glm::vec3(vertices[arrIndex + 3], vertices[arrIndex + 4], vertices[arrIndex + 5]);
}

glm::vec2 VertexConverter::getVertexTexCoord(float vertices[], std::size_t arrIndex) {
	return glm::vec2(vertices[arrIndex + 6], vertices[arrIndex + 7]);
}

glm::vec3 VertexConverter::getVertexTangent(float vertices[], std::size_t arrIndex) {
	return glm::vec3(vertices[arrIndex + 8], vertices[arrIndex + 9], vertices[arrIndex + 10]);
}

glm::vec3 VertexConverter::getVertexBitangent(float vertices[], std::size_t arrIndex) {
	return glm::vec3(vertices[arrIndex + 11], vertices[arrIndex + 12], vertices[arrIndex + 13]);
}