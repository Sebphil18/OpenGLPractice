#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VertexData {
	vec3 gNormal;
	vec2 gTexCoord;
	vec3 gTangent;
	vec3 gBitangent;

	vec4 gTangentLightPos;
	vec3 gTangentPos;
	vec3 gTangentViewDir;

} vertexIn[];

out FragmentData {
	vec3 fNormal;
	vec2 fTexCoord;
	vec3 fTangent;
	vec3 fBitangent;

	vec4 fTangentLightPos;
	vec3 fTangentPos;
	vec3 fTangentViewDir;

} fragmentOut;

void main() {
	
	for(unsigned int i = 0; i < 3; i++) {
		gl_Position = gl_in[i].gl_Position;

		fragmentOut.fNormal = vertexIn[i].gNormal;
		fragmentOut.fTexCoord = vertexIn[i].gTexCoord;
		fragmentOut.fTangent = vertexIn[i].gTangent;
		fragmentOut.fBitangent = vertexIn[i].gBitangent;

		fragmentOut.fTangentLightPos = vertexIn[i].gTangentLightPos;
		fragmentOut.fTangentPos = vertexIn[i].gTangentPos;
		fragmentOut.fTangentViewDir = vertexIn[i].gTangentViewDir;

		EmitVertex();
	}

	EndPrimitive();
}