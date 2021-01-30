#version 460 core

#define SEB_NUM_LAYERS 2

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;
layout(location = 3) in vec3 vTangent;
layout(location = 4) in vec3 vBitangent;

layout(std140) uniform matrices {
	mat4 projectionMatrix;		// 64
	mat4 viewMatrix;			// 64 * 2
	mat4 worldMatrix;			// 64 * 3
	mat4 normalMatrix;			// 64 * 4
};

uniform vec3 viewPos;
uniform mat4 dirLightSpaceMat;

uniform float layerSlope = 1;
uniform float blend = 2;
uniform float layerLevel = 0;

out VertexData {

	vec3 gPosition;
	vec3 gNormal;
	vec2 gTexCoord;
	vec3 gTangent;
	vec3 gBitangent;

	vec4 gTangentLightPos;
	vec3 gTangentPos;
	vec3 gTangentViewPos;

	mat3 tbnMatrix;

	float gLayerWeights[SEB_NUM_LAYERS];

} vertexOut;

void setUpWeights();

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
	vertexOut.gTangentViewPos = tbnMatrix * viewPos;

	setUpWeights();

	gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(vPosition, 1.0);
}

void setUpWeights() {

	for(unsigned int i = 0; i < SEB_NUM_LAYERS; i++) {
		
		/*
		float height = fragmentIn.fPosition.y - layerLevel;
		height = height >= 0 ? height : 0;
		float heightFactor = 1;
		if(layerLevel*blend != 0) {
			heightFactor = min(height / (layerLevel * blend), 1);
		}

		float slope = 1 - dot(vec3(0, 1, 0), fragmentIn.fNormal) * layerSlope;
		float slopeFactor = slope;

		float weight = heightFactor + slope;

		if(height > layerLevel) {
			weight = heightFactor;
		}
		weight = clamp(weight, 0, 1);
		*/

		float height = vertexOut.gPosition.y - layerLevel;
		height = height >= 0 ? height : 0;

		float heightFactor = 1;
		if(layerLevel*blend != 0) {
			heightFactor = min(height / (layerLevel * blend), 1);
		}

		float slope = 1 - dot(vec3(0, 1, 0), vertexOut.gNormal) * layerSlope;
		float slopeFactor = slope;

		float weight = heightFactor + slope;

		if(height > layerLevel) {
			weight = heightFactor;
		}
		weight = clamp(weight, 0, 1);

		vertexOut.gLayerWeights[i] = weight;

	}

}