#version 460 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 3) in vec3 vTangent;
layout(location = 4) in vec3 vBitangent;

layout(std140) uniform matrices{
	mat4 projectionMatrix;		// 64
	mat4 viewMatrix;			// 64 * 2
	mat4 worldMatrix;			// 64 * 3
	mat4 normalMatrix;			// 64 * 4
};

uniform vec3 viewPos;
uniform mat4 dirLightSpaceMat;

out vec3 gPosition;
out vec3 gNormal;
out vec4 gLightPosition;

out VertexData {
	vec3 gPosition;
	vec3 gNormal;
	vec3 gTangent;
	vec3 gBitangent;
	mat3 tbnMatrix;
	vec4 gLightPosition;
	vec3 gViewPos;
} vertexOut;

void main() {
	
	vec3 worldTan = vec3(worldMatrix * vec4(vTangent, 0));
	vec3 worldBiTan = vec3(worldMatrix * vec4(vBitangent, 0));
	vec3 worldNormal = vec3(worldMatrix * vec4(vNormal, 0));
	vec3 T = normalize(worldTan);
	vec3 B = normalize(worldBiTan);
	vec3 N = normalize(worldNormal);
	
	vertexOut.tbnMatrix = mat3(T, B, N);

	vertexOut.gPosition = vec3(worldMatrix * vec4(vPosition, 1));
	vertexOut.gLightPosition = dirLightSpaceMat * worldMatrix * vec4(vPosition, 1.0);
	vertexOut.gViewPos = viewPos;
	
	vertexOut.gNormal = mat3(normalMatrix) * vNormal;
	vertexOut.gTangent = vTangent;
	vertexOut.gBitangent = vBitangent;

	gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(vPosition, 1.0);
}