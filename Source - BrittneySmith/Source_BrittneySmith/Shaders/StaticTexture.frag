#version 460 core

// Vertex Shader Inputs
in vec2 FragTexCoords;

// Uniform Inputs
uniform sampler2D ImageTexture;

// Output
out vec4 FinalColor;

void main()
{
    FinalColor = texture(ImageTexture, FragTexCoords);
}