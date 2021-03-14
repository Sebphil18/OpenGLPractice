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
	setNoiseSettings();

	pushbackNewMesh(255*255);

	fillHeightMap();
	generateMeshData();
}

void TerrainModel::setNoiseSettings() {

	noiseGenerator.setFrequency(0.1);
	noiseGenerator.setAmpMultiplier(0.8);
	noiseGenerator.setOcataves(8);
}

void TerrainModel::update(bool updateHeightmap) {

	if (updateHeightmap)
		fillHeightMap();

	updateVertices();
}

void TerrainModel::resize(unsigned int lengthX, unsigned int lengthY) {

	heightMap.resize(lengthX, lengthY);

	fillHeightMap();
	generateMeshData();
}

void TerrainModel::fillHeightMap() {

	int lengthX = heightMap.getLengthX();
	int lengthY = heightMap.getLengthY();

	for (int x = 0; x < lengthX; x++) {
		for (int y = 0; y < lengthY; y++) {

			float value = noiseGenerator.fractalNoise(x + 123, y + 643);
			heightMap.setValue(x, y, value);

		}
	}
}

void TerrainModel::generateMeshData() {

	setMapDimensions();

	std::vector<unsigned int> indices;
	std::vector<Vertex> vertices;
	indices.reserve(mapDimensions.x * mapDimensions.y);
	vertices.reserve(mapDimensions.x * mapDimensions.y);

	generateIndices(indices);
	generateVertices(vertices);
	generateVerticesTBN(vertices);

	Mesh& mesh = getLastMesh();
	mesh.setData(vertices, indices);
}

void TerrainModel::updateVertices() {

	std::vector<Vertex> vertices;
	vertices.reserve(mapDimensions.x * mapDimensions.y);

	generateVertices(vertices);
	generateVerticesTBN(vertices);

	Mesh& mesh = getLastMesh();
	mesh.updateVertices(vertices, vertices.begin(), vertices.end());
}

void TerrainModel::setMapDimensions() {
	mapDimensions.x = heightMap.getLengthX();
	mapDimensions.y = heightMap.getLengthY();
}

void TerrainModel::generateVertices(std::vector<Vertex>& vertices) {

	for (std::uint32_t x = 0; x < mapDimensions.x; x++) {
		for (std::uint32_t z = 0; z < mapDimensions.y; z++) {

			float y = heightMap.getValue(x, z);

			Vertex vertex = generateVertex(x, y, z);
			vertices.push_back(vertex);

		}
	}
}

Vertex TerrainModel::generateVertex(std::uint32_t x, float y, std::uint32_t z) {

	Vertex vertex;

	vertex.position = glm::vec3(x * scale, y * scale * 5, z * scale);
	vertex.normal = glm::vec3(0, 0, 0);
	vertex.tangent = glm::vec3(1, 0, 0);
	vertex.bitangent = glm::vec3(1, 0, 0);
	vertex.texCoord = glm::vec2(vertex.position.x, vertex.position.z);

	return vertex;
}

void TerrainModel::generateVerticesTBN(std::vector<Vertex>& vertices) {

	for (std::uint32_t x = 0; x < mapDimensions.x; x++) {
		for (std::uint32_t y = 0; y < mapDimensions.y; y++) {

			glm::vec2 vertexPoint = glm::vec2(x, y);
			VertexPointContainer vertexPointCon = constructVertexPointCon(vertexPoint, vertices);

			std::array<glm::vec3, 3> tbn = getVertexTBN(vertexPoint, vertices);

			vertexPointCon.vertex.tangent = tbn[0];
			vertexPointCon.vertex.bitangent = tbn[1];
			vertexPointCon.vertex.normal = tbn[2];

		}
	}
}

std::array<glm::vec3, 3> TerrainModel::getVertexTBN(const glm::vec2& vertexPoint, std::vector<Vertex>& vertices) {

	VertexPointContainer vertexPointCon = constructVertexPointCon(vertexPoint, vertices);

	std::array<glm::vec3, 3> tbn = sampleVertexTBN(vertexPointCon, vertices);
	normalizeTBN(tbn);

	return tbn;
}

VertexPointContainer TerrainModel::constructVertexPointCon(const glm::vec2& vertexPoint, std::vector<Vertex>& vertices) {

	std::size_t index = get1DIndex(vertexPoint, mapDimensions.y);
	Vertex& vertex = vertices[index];

	return { vertex, vertexPoint };
}

std::array<glm::vec3, 3> TerrainModel::sampleVertexTBN(const VertexPointContainer& vertexPointCon, const std::vector<Vertex>& vertices) {
	
	std::array<glm::vec3, 3> tbn{ glm::vec3(0), glm::vec3(0), glm::vec3(0) };

	for (std::uint32_t neighborIndex = 0; neighborIndex < 6; neighborIndex++) {

		Triangle neighborTriangle = getNeighborTriangle(vertexPointCon, neighborIndex, vertices);

		std::array<const glm::vec3, 2> tangents = neighborTriangle.getTangents();
		tbn[0] += tangents[0];
		tbn[1] += tangents[1];
		tbn[2] += neighborTriangle.getNormal();
	}

	return tbn;
}

// neighbor index determines what neighbor triangle to choose: 0 - east; 1 - south east; 2 - south; ... (offsets stored in "neighbors" array)
Triangle TerrainModel::getNeighborTriangle(const VertexPointContainer& vertexPointCon, std::uint32_t neighborIndex, const std::vector<Vertex>& vertices) {

	const glm::vec2 neighborPoint1 = vertexPointCon.vertexPoint + neighbors[neighborIndex];
	const glm::vec2 neighborPoint2 = vertexPointCon.vertexPoint + neighbors[(neighborIndex + 1) % 6];

	if (isOutOfBounds(neighborPoint1) || isOutOfBounds(neighborPoint2))
		return Triangle();

	const Vertex& currentVertex = vertexPointCon.vertex;
	const Vertex& neighborVertex1 = vertices[get1DIndex(neighborPoint1, mapDimensions.y)];
	const Vertex& neighborVertex2 = vertices[get1DIndex(neighborPoint2, mapDimensions.y)];

	return Triangle(currentVertex, neighborVertex1, neighborVertex2);
}

inline bool TerrainModel::isOutOfBounds(const glm::vec2& vertexPoint) const {
	return vertexPoint.x >= mapDimensions.x || vertexPoint.y >= mapDimensions.y 
		|| vertexPoint.x < 0 || vertexPoint.y < 0;
}

void TerrainModel::normalizeTBN(std::array<glm::vec3, 3>& tbn) {
	for (int i = 0; i < 3; i++)
		tbn[i] = glm::normalize(tbn[i]);
}

void TerrainModel::generateIndices(std::vector<unsigned int>& indices) {
	for (std::uint32_t x = 0; x < mapDimensions.x - 1; x++) {
		for (std::uint32_t y = 0; y < mapDimensions.y - 1; y++) {
			std::size_t index = get1DIndex(glm::vec2(x, y), mapDimensions.y);

			indices.push_back(index);
			indices.push_back(index + 1);
			indices.push_back(index + mapDimensions.y);

			indices.push_back(index + 1);
			indices.push_back(index + mapDimensions.y + 1);
			indices.push_back(index + mapDimensions.y);
		}
	}
}

std::size_t TerrainModel::get1DIndex(glm::vec2 vertexPoint, std::uint32_t lengthY) {
	return static_cast<std::size_t>(vertexPoint.x) * lengthY + static_cast<std::size_t>(vertexPoint.y);
}

float* const TerrainModel::getScalePtr() {
	return &scale;
}

HeightMap* const TerrainModel::getHeightMapPtr() {
	return &heightMap;
}

FractalNoiseGenerator* const TerrainModel::getNoiseGeneratorPtr() {
	return &noiseGenerator;
}