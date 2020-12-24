#include "terrainmodel/TerrainModel.h"
#include <iostream>

const glm::vec2 TerrainModel::neighbors[]{
	glm::vec2(0, 1),
	glm::vec2(1, 1),
	glm::vec2(1, 0),
	glm::vec2(0, -1),
	glm::vec2(-1, -1),
	glm::vec2(-1, 0)
};

TerrainModel::TerrainModel(): heightMap(255, 255), scale(0.2) {
	noiseGenerator.setAmplitude(8);
	pushbackNewMesh(255*255);
	setNoiseSettings();
	fillHeightMap();
	generateMeshData();
}

void TerrainModel::setNoiseSettings() {
	noiseGenerator.setFrequency(0.1);
	noiseGenerator.setAmpMultiplier(0.8);
	noiseGenerator.setOcataves(8);
}

void TerrainModel::update(bool updateHeightmap) {
	if (updateHeightmap) {
		fillHeightMap();
	}
	updateVertices();
}

void TerrainModel::resize(unsigned int lengthX, unsigned int lengthY) {
	std::uint32_t oldLengthX = heightMap.getLengthX();
	std::uint32_t oldLengthY = heightMap.getLengthY();
	heightMap.resize(lengthX, lengthY);
	fillHeightMap();
	generateMeshData();
}

void TerrainModel::fillHeightMap() {
	int lengthX = heightMap.getLengthX();
	int lengthY = heightMap.getLengthY();
	for (int x = 0; x < lengthX; x++) {
		for (int y = 0; y < lengthY; y++) {
			float value = noiseGenerator.fractalNoise(x, y);
			heightMap.setValue(x, y, value);
		}
	}
}

void TerrainModel::generateMeshData() {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	int lengthX = heightMap.getLengthX();
	int lengthY = heightMap.getLengthY();
	glm::vec2 mapDimensions(lengthX, lengthY);

	generateVertices(mapDimensions, vertices);
	generateNormals(mapDimensions, vertices);

	generateIndices(mapDimensions, indices);

	Mesh& mesh = getLastMesh();
	mesh.setData(vertices, indices);
}

void TerrainModel::updateVertices() {
	std::vector<Vertex> vertices;
	int lengthX = heightMap.getLengthX();
	int lengthY = heightMap.getLengthY();
	glm::vec2 mapDimensions(lengthX, lengthY);

	generateVertices(mapDimensions, vertices);
	generateNormals(mapDimensions, vertices);

	Mesh& mesh = getLastMesh();
	mesh.updateVertices(vertices, vertices.begin(), vertices.end());
}

void TerrainModel::generateVertices(const glm::vec2& mapDimensions, std::vector<Vertex>& vertices) {
	for (std::uint32_t x = 0; x < mapDimensions.x; x++) {
		for (std::uint32_t y = 0; y < mapDimensions.y; y++) {
			float value = heightMap.getValue(x, y);
			Vertex vertex;
			vertex.position = glm::vec3(x * scale, value, y * scale);
			vertex.texCoord = glm::vec2(x * 0.2, y * 0.2);

			vertex.normal = glm::vec3(0, 0, 0);
			vertex.tangent = glm::vec3(1, 0, 0);
			vertex.bitangent = glm::vec3(1, 0, 0);
			vertices.push_back(vertex);
		}
	}
}

void TerrainModel::generateNormals(const glm::vec2& mapDimensions, std::vector<Vertex>& vertices) {

	for (std::uint32_t x = 0; x < mapDimensions.x; x++) {
		for (std::uint32_t y = 0; y < mapDimensions.y; y++) {
			glm::vec2 currentIndex = glm::vec2(x, y);
			calculateNormal(currentIndex, vertices, mapDimensions);
		}
	}
}

void TerrainModel::calculateNormal(const glm::vec2& vertexPoint, std::vector<Vertex>& vertices, const glm::vec2& mapDimensions) {

	std::size_t index = getIndex(vertexPoint, mapDimensions.y);
	Vertex& vertex = vertices[index];
	glm::vec3& normal = vertex.normal;
	VertexPointContainer vertexPointCon = { vertex, vertexPoint };

	for (std::uint32_t i = 0; i < 6; i++)
		normal += getNeighborNormal(vertexPointCon, vertices, mapDimensions, i);

	normal = glm::normalize(normal);
}

glm::vec3 TerrainModel::getNeighborNormal(const VertexPointContainer& vertexPointCon, std::vector<Vertex>& vertices, const glm::vec2& mapDimensions, unsigned int neighbor) {
	glm::vec2 neighborPoint1 = vertexPointCon.vertexPoint + neighbors[neighbor];
	glm::vec2 neighborPoint2 = vertexPointCon.vertexPoint + neighbors[(neighbor + 1) % 6];

	if (isOutOfBounds(neighborPoint1, mapDimensions))
		return glm::vec3(0);
	if (isOutOfBounds(neighborPoint2, mapDimensions))
		return glm::vec3(0);

	const Vertex& neighborVertex1 = vertices[getIndex(neighborPoint1, mapDimensions.y)];
	const Vertex& neighborVertex2 = vertices[getIndex(neighborPoint2, mapDimensions.y)];

	glm::vec3 edge1 = neighborVertex1.position - vertexPointCon.vertex.position;
	glm::vec3 edge2 = neighborVertex2.position - vertexPointCon.vertex.position;
	return glm::cross(edge1, edge2);
}

inline bool TerrainModel::isOutOfBounds(const glm::vec2& vertexPoint, const glm::vec2& mapDimensions) const {
	return vertexPoint.x >= mapDimensions.x || vertexPoint.y >= mapDimensions.y || vertexPoint.x < 0 || vertexPoint.y < 0;
}

void TerrainModel::generateIndices(glm::vec2 mapDimensions, std::vector<unsigned int>& indices) {
	for (std::uint32_t x = 0; x < mapDimensions.x - 1; x++) {
		for (std::uint32_t y = 0; y < mapDimensions.y - 1; y++) {
			std::size_t index = getIndex(glm::vec2(x, y), mapDimensions.y);

			indices.push_back(index);
			indices.push_back(index + 1);
			indices.push_back(index + mapDimensions.y);

			indices.push_back(index + 1);
			indices.push_back(index + mapDimensions.y + 1);
			indices.push_back(index + mapDimensions.y);
		}
	}
}

std::size_t TerrainModel::getIndex(glm::vec2 vertexPoint, unsigned int lengthY) {
	return static_cast<std::size_t>(vertexPoint.x) * lengthY + static_cast<std::size_t>(vertexPoint.y);
}

HeightMap* TerrainModel::getHeightMapPtr() {
	return &heightMap;
}

FractalNoiseGenerator* const TerrainModel::getNoiseGeneratorPtr() {
	return &noiseGenerator;
}