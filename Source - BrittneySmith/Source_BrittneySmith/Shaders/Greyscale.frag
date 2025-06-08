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

    // Luminosity Method
    float greyScale = (textureColor.r * 0.3f) + (textureColor.g * 0.11f) + (textureColor.b * 0.59f);
    
    // Greyscale by averaging
    // float greyScale = (textureColor.r + textureColor.g + textureColor.b) / 3;

    FinalColor = vec4(vec3(greyScale), 1.0f);
}