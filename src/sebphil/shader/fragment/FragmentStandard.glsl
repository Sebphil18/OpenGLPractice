#version 460 core

out vec4 color;

in vec3 fPos;

void main() {
	color = vec4(fPos, 1);
}