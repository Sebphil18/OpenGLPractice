#version 460 core

in vec4 fPos;

uniform vec3 lightPos;
uniform float far;

void main() {
	// add bias (shrinks surface along the normal)
	float bias = 0.6;
	float lightDist = distance(lightPos, fPos.xyz) + bias;

	// map depth to [0;1]
	lightDist = lightDist / far;
	gl_FragDepth = lightDist;
}