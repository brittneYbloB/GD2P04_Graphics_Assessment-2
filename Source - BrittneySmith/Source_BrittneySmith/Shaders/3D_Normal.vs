#version 460 core

// Vertex data interpretation
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in vec3 Normal;

// Inputs
uniform mat4 PVMMatrix;
uniform mat4 Model;

// Outputs to Fragment Shader
out vec2 FragTexCoords;
out vec3 LocalPos;
out vec3 FragNormal;
out vec3 FragPos;

void main()
{
	LocalPos = Position;

	// Calculate the vertex position
	gl_Position = PVMMatrix * vec4(Position, 1.0f);

	// Pass through the vertex information
	FragTexCoords = TexCoords;
	FragNormal = mat3(transpose(inverse(Model))) * Normal;
	FragPos = vec3(Model * vec4(Position, 1.0f));
}