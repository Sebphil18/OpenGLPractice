#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VertexData {
	vec3 gPosition;
	vec3 gNormal;
	vec2 gTexCoord;
	vec3 gTangent;
	vec3 gBitangent;
	vec4 gLightPosition;
} vertexIn[];

out FragmentData {
	vec3 fPosition;
	vec3 fNormal;
	vec2 fTexCoord;
	vec3 fTangent;
	vec3 fBitangent;
	vec4 fLightPosition;
} fragmentOut;

void main() {
	
	for(unsigned int i = 0; i < 3; i++) {
		gl_Position = gl_in[i].gl_Position;

		fragmentOut.fPosition = vertexIn[i].gPosition;
		fragmentOut.fNormal = vertexIn[i].gNormal;
		fragmentOut.fTexCoord = vertexIn[i].gTexCoord;
		fragmentOut.fTangent = vertexIn[i].gTangent;
		fragmentOut.fBitangent = vertexIn[i].gBitangent;
		fragmentOut.fLightPosition = vertexIn[i].gLightPosition;

		EmitVertex();
	}

	EndPrimitive();
}