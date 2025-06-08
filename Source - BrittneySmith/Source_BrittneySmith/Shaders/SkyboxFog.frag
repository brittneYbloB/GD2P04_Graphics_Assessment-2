#version 460 core

// Vertex Shader Inputs
in vec3 FragTexCoords;
//in vec2 FragTexCoords;
in vec3 LocalPos;
in vec3 FragPos;

// Uniform Inputs
uniform samplerCube Texture0;
uniform vec3 CameraPos;	

// Output
out vec4 FinalColor;

// FOG
uniform float FogStart = 1900.0f;
uniform float FogEnd = 2000.0f;
uniform vec4 FogColor = vec4(0.4f, 0.4f, 0.4f, 1.0f);


void main()
{
	float Dist = distance(FragPos, CameraPos);
	
	float FogFactor = (Dist - FogStart) / FogEnd;
	FogFactor = 1 - FogFactor;
	FogFactor = clamp(FogFactor, 0.0f, 0.97f);
	

	vec4 Color = texture(Texture0, FragTexCoords);
	//FinalColor = texture(Texture0, FragTexCoords);
	
	FinalColor = mix(Color, FogColor, FogFactor);
}