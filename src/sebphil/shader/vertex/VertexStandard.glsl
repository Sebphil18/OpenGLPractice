#version 460 core

layout(location = 0) in vec3 vPosition;

out vec3 gPos;

void main() {
	gl_Position = vec4(vPosition.x, vPosition.y, vPosition.z, 1);
	gPos = vec3(1, 0, 0);
}