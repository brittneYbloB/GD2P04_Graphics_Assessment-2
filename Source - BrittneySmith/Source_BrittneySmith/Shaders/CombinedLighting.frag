#version 460 core

#define MAX_POINT_LIGHTS 4

struct PointLight
{
	vec3 LightPos;
	vec3 LightColor;
	float AmbientStrength;
	float LightSpecularStrength;
};

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
uniform bool bIsDirectionalLightOn;
uniform bool bArePointLightsLightOn;

// Multiple Point Lights

uniform sampler2D ImageTexture;

uniform vec3 CameraPos;									// Part of specular
uniform vec3 AmbientColor;								// Generally white (all color channels)
uniform float Shininess;								// Part of specular
uniform PointLight PointLights[MAX_POINT_LIGHTS];

uniform DirLight dirLight;								// Directional lighting

// BlinnPhong
uniform float AmbientStrength;	
uniform vec3 LightColor;
uniform vec3 LightPos;
uniform float LightSpecularStrength;

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

	return Light;
}


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
	vec3 LightOutput = vec3(0.0f, 0.0f, 0.0f);

	// Calculate each of the point lights and add the results
	if (bArePointLightsLightOn)
	{
		for (int i = 0; i < MAX_POINT_LIGHTS; i++)
		{
			LightOutput += CalculateLight_Point(PointLights[i]);
		}
	}

	// Calculate directional light
	if (bIsDirectionalLightOn)
	{
		LightOutput += CalculateLight_Direction(dirLight);
	}

	// Calculate the final color
	FinalColor = vec4(LightOutput, 1.0f) * texture(ImageTexture, FragTexCoords);
}