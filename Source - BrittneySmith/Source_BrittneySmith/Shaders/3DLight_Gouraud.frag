#version 460 core

// Vertex Shader Inputs
in vec2 FragTexCoords;
in vec3 FragNormal;
in vec3 FragPos;
in vec4 Light;


// Uniform Inputs
uniform sampler2D ImageTexture;

// Output
out vec4 FinalColor;


void main()
{
	// Calculate the final color
	FinalColor = Light * texture(ImageTexture, FragTexCoords);
}