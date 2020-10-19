#version 460 core

in vec3 fNormal;
in vec3 fPosition;

uniform vec3 viewPosition;
uniform samplerCube skybox;

out vec4 color;

void main() {
	
	vec3 viewDir = normalize(fPosition - viewPosition);
	vec3 refDir = reflect(viewDir, fNormal);
	color = texture(skybox, refDir);

}