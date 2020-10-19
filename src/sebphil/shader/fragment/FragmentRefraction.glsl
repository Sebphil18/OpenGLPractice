#version 460 core

in vec3 fNormal;
in vec3 fPosition;

uniform float n1;
uniform float n2;

uniform vec3 viewPosition;

uniform samplerCube skybox;

out vec4 color;

void main() {
	vec3 viewDir = normalize(fPosition - viewPosition);
	vec3 refDir = refract(viewDir, fNormal, n1 / n2);
	color = texture(skybox, refDir);
}