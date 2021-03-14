#version 460 core

#define MAX_NUMBER_OF_DIRLIGHTS 5
#define MAX_NUMBER_OF_SHADOW_DIR_LIGHTS 1
#define MAX_NUMBER_OF_POINTLIGHTS 10
#define MAX_NUMBER_OF_SHADOW_POINT_LIGHTS 5

#define LAYER_COUNT 1

struct Material{
	float shininess;

	vec4 diffuseColor;
	vec4 specularColor;
	vec4 ambientColor;
	
	sampler2D diffuse0;
	sampler2D diffuse1;

	sampler2D specular0;

	sampler2D ambient0;

	sampler2D normal0;
	sampler2D normal1;

	sampler2D depth0;
	sampler2D depth1;
};

struct DirectionLight {

	vec3 direction;

	vec3 diffuseColor;
	vec3 specularColor;
	vec3 ambientColor;

};

struct PointLight {

	float constantK;
	float linearK;
	float quadraticK;

	vec3 position;

	vec3 diffuseColor;
	vec3 specularColor;
	vec3 ambientColor;

};

uniform Material material;

uniform int dirLightsCount;
uniform int shadowDirLightsCount;
uniform int pointLightsCount;
uniform int shadowPointLightsCount;

uniform float far;

uniform DirectionLight dirLights[MAX_NUMBER_OF_DIRLIGHTS];
uniform DirectionLight shadowDirLights[MAX_NUMBER_OF_SHADOW_DIR_LIGHTS];
uniform PointLight pointLights[MAX_NUMBER_OF_POINTLIGHTS];
uniform PointLight shadowPointLights[MAX_NUMBER_OF_SHADOW_POINT_LIGHTS];

uniform sampler2D shadowMap;
uniform samplerCube pointShadowMap;

uniform vec3 viewPos;

uniform float parallaxStrength = 0.08;
uniform int minDepthLayers = 20;
uniform int maxDepthLayers = 40;

uniform float minShadowBias = 0.005;

uniform float textureScaling = 0.4;

in FragmentData {

	vec3 fPosition;
	vec3 fNormal;
	vec2 fTexCoord;
	vec3 fTangent;
	vec3 fBitangent;

	vec4 fTangentLightPos;
	vec3 fTangentPos;
	vec3 fTangentViewPos;

	mat3 tbnMatrix;

	float fLayerWeights[1];

} fragmentIn;

out vec4 color;

vec4 getDiffuseColor(vec2 texCoord);
vec4 getSpecularColor(vec2 texCoord);
vec4 getAmbientColor(vec2 texCoord);
vec2 prallaxMapping(vec2 texCoord, vec3 viewDir, float viewLength);
float getDepth(vec2 texCoord);
vec4 getMaterialColor(vec4 baseColor, sampler2D texture2d, vec2 texCoord);

vec3 getNormal(vec2 texCoord);

vec4 getDirLightColor(DirectionLight light, vec3 normal, vec3 viewDir, vec4 diffuseColor, vec4 specularColor, vec4 ambientColor, bool castShadow);
vec4 getPointLightColor(PointLight light, vec3 normal, vec3 viewDir, vec3 fPosition, vec4 diffuseColor, vec4 specularColor, vec4 ambientColor, bool castShadow);
float calcShadow(vec4 fLightPosition, vec3 normal, vec3 lightDir);
float calcPointShadow(vec3 lightPosition, vec3 normal);

// TODO: rework parallax mapping and diffuseColor to work with layer system

void main() {

	float viewLength = length(fragmentIn.fTangentViewPos - fragmentIn.fTangentPos);
	vec3 viewDir = normalize(fragmentIn.fTangentViewPos - fragmentIn.fTangentPos);
	
	float weight = fragmentIn.fLayerWeights[0];

	vec2 texCoord = prallaxMapping(fragmentIn.fTexCoord * textureScaling, viewDir, viewLength);

	// Normal-Mapping; "normal" is in tangent-space
	vec3 normal = getNormal(texCoord);

	vec4 diffuseColor = getDiffuseColor(texCoord);
	vec4 specularColor = getSpecularColor(texCoord);
	vec4 ambientColor = getAmbientColor(texCoord);

	vec4 result = vec4(0, 0, 0, 1);

	for(unsigned int i = 0; i < dirLightsCount; i++){
		result += getDirLightColor(dirLights[i], normal, viewDir, diffuseColor, specularColor, ambientColor, false);
	}

	// ShadowDirLight
	for(unsigned int i = 0; i < shadowDirLightsCount; i++)  {
		result += getDirLightColor(shadowDirLights[i], normal, viewDir, diffuseColor, specularColor, ambientColor, true);
	}

	for(unsigned int i = 0; i < pointLightsCount; i++){
		result += getPointLightColor(pointLights[i], normal, viewDir, fragmentIn.fTangentPos, diffuseColor, specularColor, ambientColor, false);
	}

	// ShadowPointLight
	for(unsigned int i = 0; i < shadowPointLightsCount; i++) {
		result += getPointLightColor(shadowPointLights[i], normal, viewDir, fragmentIn.fTangentPos, diffuseColor, specularColor, ambientColor, true);
	}

	color = result;
}

vec4 getDiffuseColor(vec2 texCoord) {
	vec4 diffuseTexture1 = getMaterialColor(material.diffuseColor, material.diffuse0, texCoord);
	vec4 diffuseTexture2 = getMaterialColor(material.diffuseColor, material.diffuse1, texCoord);

	vec4 diffuseColor = mix(diffuseTexture1, diffuseTexture2, fragmentIn.fLayerWeights[0]);

	return diffuseColor;
}

vec4 getSpecularColor(vec2 texCoord) {
	vec4 specularColor = getMaterialColor(material.specularColor, material.specular0, texCoord);
	return specularColor;
}

vec4 getAmbientColor(vec2 texCoord) {
	vec4 ambientColor = getMaterialColor(material.ambientColor, material.ambient0, texCoord);
	return ambientColor;
}

// TODO: refactor!
vec2 prallaxMapping(vec2 texCoord, vec3 viewDir, float viewLength) {

	const float strength = parallaxStrength;
	const int minLayers = minDepthLayers;
	const int maxLayers = maxDepthLayers;

	// number of layers grows exponentiell with decreasing distance to fragment
	const float distantLayers = exp(-viewLength/20) * maxDepthLayers;
	float layers = max(minDepthLayers, distantLayers);

	// steeper viewing angle means more samples
	if(viewLength < 7) {
		const float viewLayers = (1 - dot(viewDir, vec3(0, 0, 1))) * maxDepthLayers * 1.5;
		layers = max(distantLayers, viewLayers);
	}

	// max depth of heightfield is 1
	const float layerDepth = 1 / layers;
	float currentLayerDepth = 0;

	vec2 offset = viewDir.xy * strength;
	vec2 deltaOffset = offset / layers;

	vec2 currentTexCoord = texCoord;
	float currentDepth = getDepth(currentTexCoord);

	while(currentLayerDepth < currentDepth) {
		currentTexCoord -= deltaOffset;
		currentDepth = getDepth(currentTexCoord);
		currentLayerDepth += layerDepth;
	}

	vec2 prevTexCoord = currentTexCoord + deltaOffset;

	float underneathDepth = currentDepth - currentLayerDepth;
	float overDepth = 1 - texture(material.depth0, prevTexCoord).r - currentLayerDepth + layerDepth;

	float weight = underneathDepth / (underneathDepth - overDepth);

	vec2 finalTexCoord = mix(prevTexCoord, currentTexCoord, weight);

	return finalTexCoord;
}

float getDepth(vec2 texCoord) {
	float depth1 = 1 - texture(material.depth0, texCoord).r;
	float depth2 = 1 - texture(material.depth1, texCoord).r;
	return mix(depth1, depth2, fragmentIn.fLayerWeights[0]);
}

vec4 getMaterialColor(vec4 baseColor, sampler2D texture2d, vec2 texCoord) {
	
	vec4 textureColor = texture(texture2d, texCoord);
	vec4 materialColor;

	if(textureColor.x == 0 && textureColor.y == 0 && textureColor.z == 0) {
		materialColor = baseColor;
	} else {
		materialColor = textureColor;
	}

	return materialColor;
}

vec3 getNormal(vec2 texCoord) {

	vec3 normal = normalize(fragmentIn.fNormal);

	vec3 normalMapCol1 = texture(material.normal0, texCoord).rgb;
	vec3 normalMapCol2 = texture(material.normal1, texCoord).rgb;

	vec3 normalMapCol = mix(normalMapCol1, normalMapCol2, fragmentIn.fLayerWeights[0]);

	if(length(normalMapCol) != 0) {
		normal = normalMapCol * 2 - 1;
		normal = normalize(normal);
	}

	return normal;
}

vec4 getDirLightColor(DirectionLight light, vec3 normal, vec3 viewDir, vec4 diffuseColor, vec4 specularColor, vec4 ambientColor, bool castShadow) {

	// TODO: do matrix multiplication in vertexShader -> move light arrays into vertex shader and pass to fragmentShader
	vec3 lightDir = fragmentIn.tbnMatrix * normalize(-light.direction);
	vec3 halfway = normalize(lightDir + viewDir);

	float diffuse = max(dot(normal, lightDir), 0.0);
	float spec = pow(max(dot(normal, halfway), 0.0), material.shininess);

	vec4 diffuseResult = (diffuse * diffuseColor) * vec4(light.diffuseColor, 1.0f);
	vec4 specularResult = spec * specularColor * vec4(light.specularColor, 1.0f);
	vec4 ambientResult = ambientColor * vec4(light.ambientColor, 1.0f);

	float shadow = castShadow == true ? calcShadow(fragmentIn.fTangentLightPos, normal, lightDir) : 1.0;

	return (diffuseResult * shadow + specularResult * shadow + ambientResult) * diffuseColor;
}

vec4 getPointLightColor(PointLight light, vec3 normal, vec3 viewDir, vec3 fPosition, vec4 diffuseColor, vec4 specularColor, vec4 ambientColor, bool castShadow){

	// TODO: do matrix multiplication in vertexShader -> move light arrays into vertex shader and pass to fragmentShader
	vec3 dirToLight = fragmentIn.tbnMatrix * normalize(light.position - fragmentIn.fPosition);
	vec3 halfway = normalize(viewDir + dirToLight);

	float distanceToLight = distance(light.position, fPosition);
	float attentuation = 1.0 / (light.constantK + light.linearK * distanceToLight + light.quadraticK * (distanceToLight * distanceToLight));

	// diffuse
	float diffuse = max(dot(normal, dirToLight), 0.0);
	vec4 diffuseResult = diffuse * diffuseColor * vec4(light.diffuseColor, 1) * attentuation;

	//specular
	float specular = pow(max(dot(normal, halfway), 0.0), material.shininess);
	vec4 specularResult = specular * specularColor * vec4(light.specularColor, 1) * attentuation;

	//ambient
	vec4 ambientResult = ambientColor * vec4(light.ambientColor, 1) * attentuation;

	float shadow = castShadow == true ? calcPointShadow(light.position, normal) : 1.0;

	return (diffuseResult * shadow + specularResult * shadow + ambientResult) * diffuseColor;
}

// returns how much the fragment is not in a Shadow (1.0: outside of Shadow; 0.0 in Shadow)
float calcShadow(vec4 fLightPosition, vec3 normal, vec3 lightDir) {

	float w = fLightPosition.w;
	vec4 ndcLightPosition = vec4(fLightPosition.x / w, fLightPosition.y / w, fLightPosition.z / w, 1.0);

	ndcLightPosition = ndcLightPosition * 0.5 + 0.5;

	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), minShadowBias);
	float shadow = 0.0;

	float shadowDepth = texture(shadowMap, ndcLightPosition.xy).r;
	float currentDepth = ndcLightPosition.z;

	// PCF (percentage closer filter)
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x < 2; x++) {
		for(int y = -1; y < 2; y++) {

			float pcfDepth = texture(shadowMap, ndcLightPosition.xy + vec2(x, y) * texelSize).r;
			shadow += pcfDepth < currentDepth - bias ? 0 : 1;

		}
	}

	shadow = shadow / 9.0;

	if(currentDepth > 1.0) {
		shadow = 1.0;
	}

	return shadow;
}

float calcPointShadow(vec3 lightPosition, vec3 normal) {
	
	float shadow = 0.0f;

	vec3 lightDir = fragmentIn.fPosition - lightPosition;

	float currentDepth = length(lightDir);

	float samples = 23;
	float radius = 0.05;
	
	vec3 sampleVecs[23] = vec3[] (
		vec3(1, 0, 0), vec3(0, 1, 0), vec3(0, 0, 1),
		vec3(-1, 0, 0), vec3(0, -1, 0), vec3(0, 0, -1),
		vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, 1, 0), vec3(-1, -1, 0),
		vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
		vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, 1, -1), vec3(0, -1, -1),
		vec3(1, 1, 1), vec3(-1, -1, -1), vec3(-1, -1, 1), vec3(-1, 1, -1), vec3(1, -1, -1)
	);

	for(unsigned int i = 0; i < samples; i++) {
		float nearestDepth = texture(pointShadowMap, lightDir + sampleVecs[i] * radius).r * far;

		if(nearestDepth > currentDepth)
			shadow += 1.0f;
	}

	shadow /= samples;

	return shadow;
}