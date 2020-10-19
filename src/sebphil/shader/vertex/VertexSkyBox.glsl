#version 460 core

layout(location = 0) in vec3 vPosition;

layout(std140) uniform matrices {
	mat4 projectionMatrix;	// 64
	mat4 viewMatrix;		// 128
};

out vec3 fTexCoord;

void main() {
	mat4 orientationMatrix = mat4(mat3(viewMatrix));
	vec4 position = projectionMatrix * orientationMatrix * vec4(vPosition, 1);

	// set depth to 1 (max value)
	gl_Position = position.xyww;
	fTexCoord = vPosition;
}