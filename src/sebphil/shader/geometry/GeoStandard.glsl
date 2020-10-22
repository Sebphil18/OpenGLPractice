#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in vec3 gPos[];

void main() {

	for(unsigned int i = 0; i < 3; i++) {
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}

	EndPrimitive();

}