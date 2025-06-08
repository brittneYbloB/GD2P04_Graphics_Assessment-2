#version 460 core

// Vertex Shader Inputs
in vec2 FragTexCoords;
in vec3 FragNormal;
in vec3 FragPos;


// Uniform Inputs
uniform sampler2D ImageTexture;
uniform vec3 CameraPos;									// Part of specular

uniform float AmbientStrength = 0.05f;					// Percentage value of ambient light (0 -> 1)
uniform vec3 AmbientColor = vec3(1.0f, 1.0f, 1.0f);		// Generally white (all color channels)

uniform vec3 LightColor = vec3(1.0f, 1.0f, 1.0f);		// Part of diffuse
uniform vec3 LightPos = vec3(-2.0f, 6.0f, 3.0f);		// Part of diffuse

uniform float LightSpecularStrength = 1.0f;				// Part of specular
uniform float Shininess = 32.0f;						// Part of specular


// Output
out vec4 FinalColor;


void main()
{
	// Light direction
	vec3 Normal = normalize(FragNormal);
	vec3 LightDir = normalize(FragPos - LightPos);	// Get the direction from the light towards the fragment and ensure it is normalized.

	// Ambient Component
	vec3 Ambient = AmbientStrength * AmbientColor;

	// Diffuse Component
	float DiffuseStrength = max(dot(Normal, -LightDir), 0.0f);	// Calculated strength of the diffuse light based on angle of light to fragment surface.
	vec3 Diffuse = DiffuseStrength * LightColor;

	// Specular Component
	vec3 ReverseViewDir = normalize(CameraPos - FragPos);
	vec3 HalfwayVector = normalize(-LightDir + ReverseViewDir);
	float SpecularReflectivity = pow(max(dot(Normal, HalfwayVector), 0.0f), Shininess);
	vec3 Specular = LightSpecularStrength * SpecularReflectivity * LightColor;

	// Combine the lighting components
	vec4 Light = vec4(Ambient + Diffuse + Specular, 1.0f);

	// Calculate the final color
	FinalColor = Light * texture(ImageTexture, FragTexCoords);
}