#version 460 core

// Vertex Shader Inputs
in vec2 FragTexCoords;
in vec3 FragNormal;
in vec3 FragPos;


// Uniform Inputs
uniform samplerCube ImageTexture;
uniform vec3 CameraPos;	

// Output
out vec4 FinalColor;


void main()
{
	// Reflection direction
	vec3 Normal = normalize(FragNormal);
	vec3 ViewDir = normalize(FragPos - CameraPos);
	vec3 ReflectDir = reflect(ViewDir, Normal);


	// Calculate the final color
	FinalColor = texture(ImageTexture, ReflectDir);		// (texture, textureCoords)
}