#version 460 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_GS_VERTEX
{
	in vec4 Position;
	in vec2 TexCoords;
	in vec3 Normal;
} gs_in[];	


uniform mat4 PVMMatrix;

float Magnitude = 0.5f;


void GenerateLine(int Index)
{
    gl_Position = gs_in[Index].Position;
	EmitVertex();

	vec3 NormalizedNormal = normalize(gs_in[Index].Normal);
	gl_Position = gs_in[Index].Position + PVMMatrix * vec4(NormalizedNormal, 0.0f) * Magnitude;
	EmitVertex();

	EndPrimitive();
}


void main()
{
	GenerateLine(0);
	GenerateLine(1);
	GenerateLine(2);
}