#version 460 core


struct DirLight
{
	vec3 Direction;
	vec3 Color;
	float AmbientStrength;
	float SpecularStrength;
};



// Vertex Shader Inputs
in vec2 FragTexCoords;
in vec3 FragNormal;
in vec3 FragPos;


// Uniform Inputs
uniform sampler2D ImageTexture;
uniform vec3 CameraPos;									// Part of specular
uniform vec3 AmbientColor;								// Generally white (all color channels)
uniform float Shininess;								// Part of specular
uniform DirLight dirLight;

// Output
out vec4 FinalColor;



vec3 CalculateLight_Direction(DirLight _directionLightElement)
{
	// Light direction
	vec3 Normal = normalize(FragNormal);
	vec3 LightDir = normalize(FragPos - _directionLightElement.Direction);	

	// Ambient Component
	vec3 Ambient = _directionLightElement.AmbientStrength * AmbientColor;

	// Diffuse Component
	float DiffuseStrength = max(dot(Normal, -LightDir), 0.0f);	
	vec3 Diffuse = DiffuseStrength * _directionLightElement.Color;

	// Specular Component
	vec3 ReverseViewDir = normalize(CameraPos - FragPos);
	vec3 HalfwayVector = normalize(-LightDir + ReverseViewDir);
	float SpecularReflectivity = pow(max(dot(Normal, HalfwayVector), 0.0f), Shininess);
	vec3 Specular = _directionLightElement.SpecularStrength * SpecularReflectivity * _directionLightElement.Color;

	// Combine the lighting components
	vec3 Light = vec3(Ambient + Diffuse + Specular);

	return Light;
}



void main()
{
	// Calculate each of the point lights and add the results
	vec3 LightOutput = vec3(0.0f, 0.0f, 0.0f);
	
	LightOutput += CalculateLight_Direction(dirLight);

	// Calculate the final color
	FinalColor = vec4(LightOutput, 1.0f) * texture(ImageTexture, FragTexCoords);
}