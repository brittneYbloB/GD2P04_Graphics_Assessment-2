#version 460 core

// Vertex data interpretation
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;
layout (location = 2) in vec2 TexCoords;

// Outputs to Fragment Shader
out vec3 FragColor;
out vec2 FragTexCoords;

void main()
{
	gl_Position = vec4(Position, 1.0f);
	FragColor = Color;
	FragTexCoords = TexCoords;
}