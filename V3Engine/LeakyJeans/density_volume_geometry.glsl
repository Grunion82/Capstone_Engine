#version 330
layout (triangles) in;
layout(triangle_strip, max_vertices = 6) out;

int maxvertexCount = 3;

//in VS_OUT{
//	vec3 fragPos;
//	vec2 tex;
//} gs_in[];

void main(){
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();
	EndPrimitive();
}