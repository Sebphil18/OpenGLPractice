#version 460 core

#define SEB_NUM_LAYERS 1

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

struct Layer {
	float maxLevel;
	float minLevel;
	float blend;
	float slope;
};

uniform vec3 viewPos;
uniform mat4 dirLightSpaceMat;

uniform Layer layers[1];

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

		Layer layer = layers[i];

		const float maxLevel = layer.maxLevel;
		const float blend = layer.blend;
		const float layerSlope = layer.slope;

		float height = vertexOut.gPosition.y - maxLevel;

		float heightFactor = 1;

		if(vertexOut.gPosition.y <= maxLevel) {
			
			if(maxLevel * blend != 0) {

				heightFactor = abs(height / (maxLevel * blend));
				heightFactor = 1 - clamp(heightFactor, 0, 1);

			} else {

				heightFactor = abs(height / (0.001 * blend));
				heightFactor = 1 - clamp(heightFactor, 0, 1);

			}

		}

		// slope = 0 when vertex is tilted 90°
		float slope = (1 - abs(vertexOut.gNormal.y)) * layerSlope;
		float slopeFactor = clamp(slope, 0, 1);

		float weight = heightFactor + slopeFactor;
		weight = clamp(weight, 0, 1);

		vertexOut.gLayerWeights[i] = weight;
	}

}