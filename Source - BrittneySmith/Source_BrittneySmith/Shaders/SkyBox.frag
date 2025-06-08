#version 460 core

// Vertex Shader Inputs
in vec3 FragTexCoords;

// Uniform Inputs
uniform samplerCube Texture0;

// Output
out vec4 FinalColor;


void main()
{
	FinalColor = texture(Texture0, FragTexCoords);
}