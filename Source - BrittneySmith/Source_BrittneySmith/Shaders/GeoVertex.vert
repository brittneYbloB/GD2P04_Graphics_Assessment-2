#version 460 core

layout (location = 0) in vec3 Position;

out VS_GS_VERTEX
{
	out vec4 Position;	// struct member/name (must match geometry shader input struct)
} vs_out;	// variable name (local to this shader)

uniform mat4 PVMMatrix;


void main()
{
	gl_Position = PVMMatrix * vec4(Position, 1.0f);

	vs_out.Position = gl_Position;
}