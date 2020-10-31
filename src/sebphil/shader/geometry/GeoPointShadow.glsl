#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

in vec3 gNormal[];

uniform mat4 lightSpaceMat[6];

out vec4 fPos;
out vec3 fNormal;

void main() {

	for(int i = 0; i < 6; i++) {

		gl_Layer = i;

		for(int j = 0; j < 3; j++) {

			fPos = gl_in[j].gl_Position;
			fNormal = gNormal[i];
			gl_Position = lightSpaceMat[i] * fPos;

			EmitVertex();
		}
		
		EndPrimitive();
	}

}