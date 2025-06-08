#version 460 core

#define MAX_POINT_LIGHTS 4

struct PointLight
{
	vec3 LightPos;
	vec3 LightColor;
	float AmbientStrength;
	float LightSpecularStrength;

	float AttenuationConstant;
	float AttenuationLinear;
	float AttenuationExponent;
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
uniform PointLight PointLights[MAX_POINT_LIGHTS];

// Output
out vec4 FinalColor;



vec3 CalculateLight_Point(PointLight _pointLightElement)
{
	// Light direction
	vec3 Normal = normalize(FragNormal);
	vec3 LightDir = normalize(FragPos - _pointLightElement.LightPos);	

	// Ambient Component
	vec3 Ambient = _pointLightElement.AmbientStrength * AmbientColor;

	// Diffuse Component
	float DiffuseStrength = max(dot(Normal, -LightDir), 0.0f);	
	vec3 Diffuse = DiffuseStrength * _pointLightElement.LightColor;

	// Specular Component
	vec3 ReverseViewDir = normalize(CameraPos - FragPos);
	vec3 HalfwayVector = normalize(-LightDir + ReverseViewDir);
	float SpecularReflectivity = pow(max(dot(Normal, HalfwayVector), 0.0f), Shininess);
	vec3 Specular = _pointLightElement.LightSpecularStrength * SpecularReflectivity * _pointLightElement.LightColor;

	// Combine the lighting components
	vec3 Light = vec3(Ambient + Diffuse + Specular);

	// Calculate and apply the attenuation
	float Distance = length(_pointLightElement.LightPos - FragPos);
	float Attenuation = _pointLightElement.AttenuationConstant + (_pointLightElement.AttenuationLinear * Distance) + (_pointLightElement.AttenuationExponent * pow(Distance, 2));
	Light /= Attenuation;

	return Light;
}



void main()
{
	// Calculate each of the point lights and add the results
	vec3 LightOutput = vec3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		LightOutput += CalculateLight_Point(PointLights[i]);
	}

	// Calculate the final color
	FinalColor = vec4(LightOutput, 1.0f) * texture(ImageTexture, FragTexCoords);
}