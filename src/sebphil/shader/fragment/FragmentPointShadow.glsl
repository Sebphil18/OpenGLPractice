#version 460 core

in vec4 fPos;
in vec3 fNormal;

uniform vec3 lightPos;
uniform float far;

void main() {
	// add bias (shrinks surface along the normal)
	vec3 lightDir = fPos.xyz - lightPos;
	// max(0.6 * (1 - dot(lightDir, fNormal)), 0.05)
	float bias = 0.6;

	float lightDist = distance(lightPos, fPos.xyz) + bias;
	// map depth to [0;1]
	lightDist = lightDist / far;
	gl_FragDepth = lightDist;
}