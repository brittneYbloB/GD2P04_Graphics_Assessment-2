#version 460 core

// Vertex Shader Inputs
in vec3 FragColor;
in vec2 FragTexCoords;

// Uniform Inputs
uniform sampler2D ImageTexture;

// Output
out vec4 FinalColor;

void main()
{
    vec4 textureColor = texture(ImageTexture, FragTexCoords);

    FinalColor = vec4(1.0 - textureColor.r,1.0 -textureColor.g,1.0 -textureColor.b,1);
}