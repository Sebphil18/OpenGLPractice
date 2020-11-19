#version 460 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;
layout(location = 3) in vec3 vTagent;
layout(location = 4) in vec3 vBitangent;

layout(std140) uniform matrices{
	mat4 projectionMatrix;		// 64
	mat4 viewMatrix;			// 64 * 2
	mat4 worldMatrix;			// 64 * 3
	mat4 normalMatrix;			// 64 * 4
};

uniform mat4 dirLightSpaceMat;

out vec3 gPosition;
out vec3 gNormal;
out vec2 gTexCoord;
out vec4 gLightPosition;

out VertexData {
	vec3 gPosition;
	vec3 gNormal;
	vec2 gTexCoord;
	vec3 gTangent;
	vec3 gBitangent;
	vec4 gLightPosition;
} vertexOut;

void main() {

	vertexOut.gTexCoord = vTexCoord;
	vertexOut.gPosition = vec3(worldMatrix * vec4(vPosition, 1));
	vertexOut.gNormal = mat3(normalMatrix) * vNormal;
	vertexOut.gTangent = vTagent;
	vertexOut.gBitangent = vBitangent;

	vertexOut.gLightPosition = dirLightSpaceMat * worldMatrix * vec4(vPosition, 1.0);

	gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(vPosition, 1.0);
}