#version 460 core

#define MAX_NUMBER_OF_DIRLIGHTS 5
#define MAX_NUMBER_OF_SHADOW_DIR_LIGHTS 1
#define MAX_NUMBER_OF_POINTLIGHTS 10
#define MAX_NUMBER_OF_SHADOW_POINT_LIGHTS 5

struct Material{
	float shininess;

	vec4 diffuseColor;
	vec4 specularColor;
	vec4 ambientColor;
	
	sampler2D diffuse0;
	sampler2D diffuse1;
	sampler2D specular0;
	sampler2D ambient0;
	sampler2D normal;
};

struct DirectionLight{

	vec3 direction;

	vec3 diffuseColor;
	vec3 specularColor;
	vec3 ambientColor;

};

struct PointLight{

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

in FragmentData {
	vec3 fPosition;
	vec3 fNormal;
	vec2 fTexCoord;
	vec3 fTangent;
	vec3 fBitangent;
	mat3 tbnMatrix;
	vec4 fLightPosition;
	vec3 fViewPos;
} fragmentIn;

out vec4 color;

vec4 getDirLightColor(DirectionLight light, vec3 normal, vec3 viewDir, vec4 diffuseColor, vec4 specularColor, vec4 ambientColor, bool castShadow);
vec4 getPointLightColor(PointLight light, vec3 normal, vec3 viewDir, vec3 fPosition, vec4 diffuseColor, vec4 specularColor, vec4 ambientColor, bool castShadow);
float calcShadow(vec4 fLightPosition, vec3 normal, vec3 lightDir);
float calcPointShadow(vec3 fPosition, vec3 lightPosition, vec3 normal);

void main() {

	vec3 viewDir = normalize(fragmentIn.fViewPos - fragmentIn.fPosition);

	vec2 texCoord1 = vec2(fragmentIn.fTexCoord.x * 0.5, fragmentIn.fTexCoord.y * 0.5);
	vec2 texCoord2 = vec2(fragmentIn.fTexCoord.x * 0.2, fragmentIn.fTexCoord.y * 0.2);
	vec4 diffTexture = texture(material.diffuse0, texCoord1);
	vec4 diffTexture2 = texture(material.diffuse1, texCoord2);

	vec4 specTexture = texture(material.specular0, fragmentIn.fTexCoord);
	vec4 ambientTexture = texture(material.ambient0, fragmentIn.fTexCoord);

	vec4 diffuseColor;

	float height = fragmentIn.fPosition.y >= 0 ? fragmentIn.fPosition.y : 0;
	float d = dot(vec3(0, 1, 0), fragmentIn.fNormal);
	if(height > 8) {
		diffuseColor = mix(diffTexture, diffTexture2,  min(0.8, (1 - (height / 5)) * d));
	} else {
		diffuseColor = mix(diffTexture, diffTexture2, (1 - (height / 8)) * d);
	}

	vec4 specularColor;
	if(specTexture.x == 0 && specTexture.y == 0 && specTexture.z == 0) {
		specularColor = material.specularColor;
	} else {
		specularColor = specTexture;
	}

	vec4 ambientColor;
	if(ambientTexture.x == 0 && ambientTexture.y == 0 && ambientTexture.z == 0) {
		ambientColor = material.ambientColor;
	} else {
		ambientColor = ambientTexture;
	}

	// Normal-Mapping
	vec3 normal = normalize(fragmentIn.fNormal);
	vec3 position = fragmentIn.fPosition;
	vec3 normalMapCol = texture(material.normal, fragmentIn.fTexCoord).rgb;
	if(length(normalMapCol) != 0) {
		normal = normalMapCol * 2 - 1;
		normal = fragmentIn.tbnMatrix * normal;
		normal = normalize(normal);
	}

	vec4 result = vec4(0, 0, 0, 1);

	for(unsigned int i = 0; i < dirLightsCount; i++){
		result += getDirLightColor(dirLights[i], normal, viewDir, diffuseColor, specularColor, ambientColor, false);
	}

	// ShadowDirLight
	for(unsigned int i = 0; i < shadowDirLightsCount; i++)  {
		result += getDirLightColor(shadowDirLights[i], normal, viewDir, diffuseColor, specularColor, ambientColor, true);
	}

	for(unsigned int i = 0; i < pointLightsCount; i++){
		result += getPointLightColor(pointLights[i], normal, viewDir, position, diffuseColor, specularColor, ambientColor, false);
	}

	// ShadowPointLight
	for(unsigned int i = 0; i < shadowPointLightsCount; i++) {
		result += getPointLightColor(shadowPointLights[i], normal, viewDir, position, diffuseColor, specularColor, ambientColor, true);
	}

	color = result;
}

vec4 getDirLightColor(DirectionLight light, vec3 normal, vec3 viewDir, vec4 diffuseColor, vec4 specularColor, vec4 ambientColor, bool castShadow) {

	// fPosition, viewPos and light.direction can be precalculated in vertex shader using transpose of tbnMatrix - only for color calculations, not for shadow!
	// precalculating these in vertex shader is much more efficient
	vec3 lightDir = normalize(-light.direction);
	vec3 halfway = normalize(lightDir + viewDir);

	float diffuse = max(dot(normal, lightDir), 0.0);
	float spec = pow(max(dot(normal, halfway), 0.0), material.shininess);

	vec4 diffuseResult = (diffuse * diffuseColor) * vec4(light.diffuseColor, 1.0f);
	vec4 specularResult = spec * specularColor * vec4(light.specularColor, 1.0f);
	vec4 ambientResult = ambientColor * vec4(light.ambientColor, 1.0f);

	float shadow = castShadow == true ? calcShadow(fragmentIn.fLightPosition, normal, lightDir) : 1.0;

	return (diffuseResult * shadow + specularResult * shadow + ambientResult) * diffuseColor;
}

vec4 getPointLightColor(PointLight light, vec3 normal, vec3 viewDir, vec3 fPosition, vec4 diffuseColor, vec4 specularColor, vec4 ambientColor, bool castShadow){

	// fPosition, viewPos and light.position can be precalculated in vertex shader using transpose of tbnMatrix - only for color calculations, not for shadow!
	// precalculating these in vertex shader is much more efficient
	vec3 dirToLight = normalize(light.position - fPosition);
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

	float shadow = castShadow == true ? calcPointShadow(fPosition, light.position, normal) : 1.0;

	return (diffuseResult * shadow + specularResult * shadow + ambientResult) * diffuseColor;
}

// returns how much the fragment is not in a Shadow (1.0: outside of Shadow; 0.0 in Shadow)
float calcShadow(vec4 fLightPosition, vec3 normal, vec3 lightDir) {

	float w = fLightPosition.w;
	vec4 ndcLightPosition = vec4(fLightPosition.x / w, fLightPosition.y / w, fLightPosition.z / w, 1.0);

	ndcLightPosition = ndcLightPosition * 0.5 + 0.5;

	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
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

float calcPointShadow(vec3 fPosition, vec3 lightPosition, vec3 normal) {
	
	float shadow = 0.0f;

	vec3 lightDir = fPosition - lightPosition;

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