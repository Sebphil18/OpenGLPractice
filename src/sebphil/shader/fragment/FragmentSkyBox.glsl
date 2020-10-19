#version 460 core

in vec3 fTexCoord;

uniform samplerCube skybox;

out vec4 color;

void main(){
	color = texture(skybox, fTexCoord);
}