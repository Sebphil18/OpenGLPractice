#include "terrainmodel/Triangle.h"

Triangle::Triangle() {
	vertex1 = nullptr;
	vertex2 = nullptr;
	vertex3 = nullptr;

	edge1 = glm::vec3(0);
	edge2 = glm::vec3(0);
	deltaTexCoord1 = glm::vec3(0);
	deltaTexCoord2 = glm::vec3(0);
}

Triangle::Triangle(const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3):
	vertex1(&vertex1), vertex2(&vertex2), vertex3(&vertex3) {

	deltaTexCoord1 = vertex2.texCoord - vertex1.texCoord;
	deltaTexCoord2 = vertex3.texCoord - vertex1.texCoord;

	edge1 = vertex2.position - vertex1.position;
	edge2 = vertex3.position - vertex1.position;
}

glm::vec3 Triangle::getNormal() const {
	return glm::cross(edge1, edge2);
}

std::array<const glm::vec3, 2> Triangle::getTangents() const {

	const float fraction = getTangentFraction();

	glm::vec3 tangent = getTangent(fraction);
	glm::vec3 bitangent = getBitangent(fraction);

	return std::array<const glm::vec3, 2>{tangent, bitangent};
}

glm::vec3 Triangle::getTangent(const float fraction) const {

	if (vertex1 == nullptr)
		return glm::vec3(0);

	glm::vec3 tangent;
	tangent.x = fraction * (deltaTexCoord2.y * edge1.x - deltaTexCoord1.y * edge2.x);
	tangent.y = fraction * (deltaTexCoord2.y * edge1.y - deltaTexCoord1.y * edge2.y);
	tangent.z = fraction * (deltaTexCoord2.y * edge1.z - deltaTexCoord1.y * edge2.z);

	return tangent;
}

glm::vec3 Triangle::getBitangent(const float fraction) const {

	if (vertex1 == nullptr)
		return glm::vec3(0);

	glm::vec3 bitangent;
	bitangent.x = fraction * (-deltaTexCoord2.x * edge1.x + deltaTexCoord1.x * edge2.x);
	bitangent.y = fraction * (-deltaTexCoord2.x * edge1.y + deltaTexCoord1.x * edge2.y);
	bitangent.z = fraction * (-deltaTexCoord2.x * edge1.z + deltaTexCoord1.x * edge2.z);

	return bitangent;
}

float Triangle::getTangentFraction() const {
	return 1 / (deltaTexCoord1.x * deltaTexCoord2.y - deltaTexCoord2.x * deltaTexCoord1.y);
}