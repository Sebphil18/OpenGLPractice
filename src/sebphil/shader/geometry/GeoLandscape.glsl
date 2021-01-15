#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VertexData {
	vec3 gPosition;
	vec3 gNormal;
	vec3 gTangent;
	vec3 gBitangent;
	mat3 tbnMatrix;
	vec4 gLightPosition;
	vec3 gViewPos;
} vertexIn[];

out FragmentData {
	vec3 fPosition;
	vec3 fNormal;
	vec3 fTangent;
	vec3 fBitangent;
	mat3 tbnMatrix;
	vec4 fLightPosition;
	vec3 fViewPos;
} fragmentOut;

void main() {
	
	for(unsigned int i = 0; i < 3; i++) {
		gl_Position = gl_in[i].gl_Position;

		fragmentOut.fPosition = vertexIn[i].gPosition;

		fragmentOut.fNormal = vertexIn[i].gNormal;
		fragmentOut.fTangent = vertexIn[i].gTangent;
		fragmentOut.fBitangent = vertexIn[i].gBitangent;

		fragmentOut.tbnMatrix = vertexIn[i].tbnMatrix;
		fragmentOut.fLightPosition = vertexIn[i].gLightPosition;

		fragmentOut.fViewPos = vertexIn[i].gViewPos;

		EmitVertex();
	}

	EndPrimitive();
}