#version 460 core

layout(location = 0) in vec3 vPos;

uniform mat4 worldMatrix;

void main() {
	gl_Position = worldMatrix * vec4(vPos, 1);
}