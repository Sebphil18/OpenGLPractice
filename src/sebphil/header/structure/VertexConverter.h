#pragma once
#include "Mesh.h"

class VertexConverter {

private:

	static Vertex getVertexAttributes(float vertices[], std::size_t arrIndex);
	static glm::vec3 getVertexPosition(float vertices[], std::size_t arrIndex);
	static glm::vec3 getVertexNormal(float vertices[], std::size_t arrIndex);
	static glm::vec2 getVertexTexCoord(float vertices[], std::size_t arrIndex);
	static glm::vec3 getVertexTangent(float vertices[], std::size_t arrIndex);
	static glm::vec3 getVertexBitangent(float vertices[], std::size_t arrIndex);

public:
	
	static std::vector<Vertex> floatArrToVertexVec(float vertices[], std::size_t size);

};
