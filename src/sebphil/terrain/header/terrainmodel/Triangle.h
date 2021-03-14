#pragma once
#include <array>
#include "glm/glm.hpp"
#include "modelstructure/Mesh.h"

class Triangle {

private:
	const Vertex* vertex1;
	const Vertex* vertex2;
	const Vertex* vertex3;

	glm::vec2 deltaTexCoord1, deltaTexCoord2;
	glm::vec3 edge1, edge2;

	float getTangentFraction() const;
	glm::vec3 getTangent(const float fraction) const;
	glm::vec3 getBitangent(const float fraction) const;

public:
	Triangle();
	Triangle(const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3);

	glm::vec3 getNormal() const;
	std::array<const glm::vec3, 2> getTangents() const;
};