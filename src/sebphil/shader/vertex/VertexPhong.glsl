#version 460 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;
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

out VertexData {

	vec3 gPosition;
	vec3 gNormal;
	vec2 gTexCoord;
	vec3 gTangent;
	vec3 gBitangent;

	vec4 gTangentLightPos;
	vec3 gTangentPos;
	vec3 gTangentViewDir;

	mat3 tbnMatrix;

} vertexOut;

void main() {
	
	vec3 worldTan = vec3(worldMatrix * vec4(vTangent, 0.0));
	vec3 worldBiTan = vec3(worldMatrix * vec4(vBitangent, 0.0));
	vec3 worldNormal = vec3(worldMatrix * vec4(vNormal, 0.0));
	vec3 T = normalize(worldTan);
	vec3 B = normalize(worldBiTan);
	vec3 N = normalize(worldNormal);

	vec3 position = vec3(worldMatrix * vec4(vPosition, 1.0));
	vec4 lightPosition = dirLightSpaceMat * worldMatrix * vec4(vPosition, 1.0);

	vertexOut.gPosition = position;
	vertexOut.gNormal = mat3(normalMatrix) * vNormal;
	vertexOut.gTexCoord = vTexCoord;
	vertexOut.gTangent = vTangent;
	vertexOut.gBitangent = vBitangent;

	vertexOut.tbnMatrix = transpose(mat3(T, B, N));
	vertexOut.gTangentLightPos = lightPosition;

	mat3 tbnMatrix = transpose(mat3(T, B, N));
	vertexOut.gTangentPos = tbnMatrix * position;
	vertexOut.gTangentViewDir = tbnMatrix *  normalize(position - viewPos);

	gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(vPosition, 1.0);
}