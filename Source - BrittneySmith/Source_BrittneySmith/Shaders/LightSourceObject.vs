#version 460 core

// Vertex data interpretation
layout (location = 0) in vec3 Position;

// Inputs
uniform mat4 PVMMatrix;


void main()
{
	// Calculate the vertex position
	gl_Position = PVMMatrix * vec4(Position, 1.0f);
}