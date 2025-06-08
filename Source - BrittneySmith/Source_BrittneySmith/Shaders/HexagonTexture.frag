#version 460 core

// Vertex Shader Inputs
in vec3 FragColor;
in vec2 FragTexCoords;

// Uniform Inputs
uniform sampler2D ImageTexture0;
uniform sampler2D ImageTexture1;
uniform float CurrentTime;

// Output
out vec4 FinalColor;



void main()
{
	FinalColor = mix(texture(ImageTexture0, FragTexCoords) * abs(sin(CurrentTime)), texture(ImageTexture1, FragTexCoords) * abs(cos(CurrentTime)), 0.5f);
}