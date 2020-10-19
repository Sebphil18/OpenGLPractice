#version 460 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;

layout(std140) uniform matrices {
	mat4 projectionMatrix;		// 64
	mat4 viewMatrix;			// 64 * 2
	mat4 worldMatrix;			// 64 * 3
	mat4 normalMatrix;			// 64 * 4
};

out vec3 fPosition;
out vec3 fNormal;

void main() {
	gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(vPosition, 1);
	fNormal = mat3(normalMatrix) * vNormal;
	fPosition = vec3(worldMatrix * vec4(vPosition, 1));
}