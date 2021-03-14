#pragma once
#include <array>
#include "heightmap/HeightMap.h"
#include "noise/FractalNoiseGenerator.h"
#include "modelstructure/Model.h"
#include "terrainmodel/Triangle.h"

struct VertexPointContainer {
	Vertex& vertex;
	const glm::vec2& vertexPoint;
};

class TerrainModel: public Model {

private:
	const static glm::vec2 neighbors[6];

	float scale;
	HeightMap heightMap;
	FractalNoiseGenerator noiseGenerator;
	glm::vec2 mapDimensions;

	void setNoiseSettings();

	void generateMeshData();
	void updateVertices();

	void setMapDimensions();

	void generateVertices(std::vector<Vertex>& vertices);
	Vertex generateVertex(std::uint32_t x, float y, std::uint32_t z);

	void generateVerticesTBN(std::vector<Vertex>& vertices);

	std::array<glm::vec3, 3> getVertexTBN(const glm::vec2& vertexPoint, std::vector<Vertex>& vertices);

	VertexPointContainer constructVertexPointCon(const glm::vec2& vertexPoint, std::vector<Vertex>& vertices);

	std::array<glm::vec3, 3> sampleVertexTBN(const VertexPointContainer& vertexPointCon, const std::vector<Vertex>& vertices);
	Triangle getNeighborTriangle(const VertexPointContainer& vertexPointCon, std::uint32_t neighborIndex, const std::vector<Vertex>& vertices);

	inline bool isOutOfBounds(const glm::vec2& vertexPoint) const;

	void normalizeTBN(std::array<glm::vec3, 3>& tbn);

	void generateIndices(std::vector<unsigned int>& indices);

	static std::size_t get1DIndex(glm::vec2 vertexPoint, unsigned int lengthX);

public:
	TerrainModel();

	void update(bool updateHeightmap = true);
	void fillHeightMap();
	void resize(unsigned int lengthX, unsigned int lengthY);

	float* const getScalePtr();
	HeightMap* const getHeightMapPtr();
	FractalNoiseGenerator* const getNoiseGeneratorPtr();

};