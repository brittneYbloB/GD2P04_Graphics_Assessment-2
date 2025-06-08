#version 460 core

// Vertex Shader Inputs
in vec2 FragTexCoords;
in vec3 LocalPos;
in vec3 FragPos;

// Uniform Inputs
uniform sampler3D Texture3D;
uniform vec3 CameraPos;	

// Output
out vec4 FinalColor;


// FOG
uniform float FogStart = 0.25f;
uniform float FogEnd = 0.75f;
uniform vec4 FogColor = vec4(0.4f, 0.4f, 0.4f, 1.0f);



void main()
{
	float Dist = distance(FragPos, CameraPos);
	
	float FogFactor = (Dist - FogStart) / FogEnd;
	FogFactor = clamp(FogFactor, 0.0f, 0.97f);

	vec4 Color = texture(Texture3D, LocalPos / 0.05f + 0.5f);			// 0.05f is what I've used as the model/mesh dependent value (was 10.0f)
	
	FinalColor = mix(Color, FogColor, FogFactor);
}