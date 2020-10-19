#version 460 core

layout(location = 0) in vec3 vPosition;

uniform mat4 transformViewMat;
uniform mat4 worldMatrix;

void main() {
	gl_Position = transformViewMat * worldMatrix * vec4(vPosition, 1.0);
}