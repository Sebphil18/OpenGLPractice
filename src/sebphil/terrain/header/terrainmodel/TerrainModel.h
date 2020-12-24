#pragma once
#include "heightmap/HeightMap.h"
#include "noise/FractalNoiseGenerator.h"
#include "modelstructure/Model.h"

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

	void setNoiseSettings();

	void generateMeshData();
	void updateVertices();
	void generateVertices(const glm::vec2& mapDimensions, std::vector<Vertex>& vertices);

	void generateNormals(const glm::vec2& mapDimensions, std::vector<Vertex>& vertices);
	void calculateNormal(const glm::vec2& vertexPoint, std::vector<Vertex>& vertices, const glm::vec2& mapDimensions);
	// TODO: to many arguments!
	glm::vec3 getNeighborNormal(const VertexPointContainer& vertexPointCon, std::vector<Vertex>& vertices, const glm::vec2& mapDimensions, unsigned int neighbor);
	inline bool isOutOfBounds(const glm::vec2& vertexPoint, const glm::vec2& mapDimensions) const;

	void generateIndices(glm::vec2 mapDimensions, std::vector<unsigned int>& indices);

	static std::size_t getIndex(glm::vec2 vertexPoint, unsigned int lengthX);

public:
	TerrainModel();

	void update(bool updateHeightmap = true);
	void fillHeightMap();
	void resize(unsigned int lengthX, unsigned int lengthY);

	HeightMap* getHeightMapPtr();
	FractalNoiseGenerator* const getNoiseGeneratorPtr();

};