#version 460 core

in vec3 fTexCoord;

uniform samplerCube skybox;

uniform vec3 specularColor = vec3(1, 1, 1);

out vec4 color;

void main(){
	color = texture(skybox, fTexCoord) * vec4(specularColor, 1);
}