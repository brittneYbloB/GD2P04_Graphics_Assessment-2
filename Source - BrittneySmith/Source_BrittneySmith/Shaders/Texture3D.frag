#version 460 core

// Vertex Shader Inputs
in vec2 FragTexCoords;
in vec3 LocalPos;

// Uniform Inputs
uniform sampler3D Texture3D;
uniform float ModelDependentValue;

// Output
out vec4 FinalColor;

void main()
{
	FinalColor = texture(Texture3D, LocalPos / 0.05f + ModelDependentValue);			// 0.05f is what I've used as the model/mesh dependent value (was 10.0f)
}