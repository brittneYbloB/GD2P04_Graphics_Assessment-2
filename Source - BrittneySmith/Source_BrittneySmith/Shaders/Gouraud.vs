#version 460 core

// Vertex data interpretation
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in vec3 Normal;

// Inputs
uniform mat4 PVMMatrix;
uniform mat4 Model;

// Uniform Inputs
uniform vec3 CameraPos;									// Part of specular

uniform float AmbientStrength = 0.05f;					// Percentage value of ambient light (0 -> 1)
uniform vec3 AmbientColor = vec3(1.0f, 1.0f, 1.0f);		// Generally white (all color channels)

uniform vec3 LightColor = vec3(1.0f, 1.0f, 1.0f);		// Part of diffuse
uniform vec3 LightPos = vec3(-2.0f, 6.0f, 3.0f);		// Part of diffuse

uniform float LightSpecularStrength = 1.0f;				// Part of specular
uniform float Shininess = 32.0f;						// Part of specular

// Outputs to Fragment Shader
out vec2 FragTexCoords;
out vec3 FragNormal;
out vec3 FragPos;
out vec4 Light;


void main()
{
	// Calculate the vertex position
	gl_Position = PVMMatrix * vec4(Position, 1.0f);

	// Pass through the vertex information
	FragTexCoords = TexCoords;
	FragNormal = mat3(transpose(inverse(Model))) * Normal;
	FragPos = vec3(Model * vec4(Position, 1.0f));

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
	vec3 ReflectedDir = reflect(LightDir, Normal);
	float SpecularReflectivity = pow(max(dot(ReverseViewDir, ReflectedDir), 0.0f), Shininess);
	vec3 Specular = LightSpecularStrength * SpecularReflectivity * LightColor;
		
	// Combine the lighting components
	Light = vec4(Ambient + Diffuse + Specular, 1.0f);
}