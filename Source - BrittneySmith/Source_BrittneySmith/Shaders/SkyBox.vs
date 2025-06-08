#version 460 core

// Vertex data interpretation
layout (location = 0) in vec3 Position;

// Inputs
uniform mat4 PVM;

// Outputs to Fragment Shader
out vec3 FragTexCoords;
out vec3 LocalPos;
out vec3 FragNormal;
out vec3 FragPos;


void main()
{
	gl_Position = PVM * vec4(Position, 1.0f);
	FragTexCoords = Position;
}