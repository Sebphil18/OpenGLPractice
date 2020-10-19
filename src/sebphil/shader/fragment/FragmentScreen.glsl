#version 330 core

out vec4 color;

in vec2 vTexCoord;

uniform sampler2D screenTexture;

void main() {
	vec4 screenColor = texture(screenTexture, vTexCoord);
	color = vec4(screenColor.r, screenColor.g, screenColor.b, 1.0);
}