#version 460 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;

uniform mat4 worldMatrix;

out vec3 gNormal;

void main() {
	gNormal = vNormal;
	gl_Position = worldMatrix * vec4(vPos, 1);
}