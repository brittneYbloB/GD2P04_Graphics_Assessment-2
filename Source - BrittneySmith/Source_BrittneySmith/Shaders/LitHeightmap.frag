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

// Multiple textures
uniform sampler2D imageSnow;
uniform sampler2D imageRocks;
uniform sampler2D imageGrass;
uniform sampler2D imageDirt;

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

	// Texture Setting
	// Grass
	if (FragPos.y < -0.48f)
	{
		// Calculate the final color
		FinalColor = vec4(LightOutput, 1.0f) * texture(imageGrass, FragTexCoords);
	}
	// Blended Grass and Dirt
	//else if()
	//{
		//vec4 c1 = texture(texture1, textureCoord1.st);
		//vec4 c2 = texture(texture2, textureCoord2.st);
		//gl_FragColor = c1*(1-thatMuchOfTexture2) + c2*thatMuchOfTexture2
	//}

	// Dirt
	else if (FragPos.y < -0.36f && 
				FragPos.y >= -0.48f)
	{
		// Calculate the final color
		FinalColor = vec4(LightOutput, 1.0f) * texture(imageDirt, FragTexCoords);
	}
	// Blended Dirt and Rocks/Stone

	// Rocks/Stone
	else if (FragPos.y < -0.24f && 
				FragPos.y >= -0.36f)
	{
		// Calculate the final color
		FinalColor = vec4(LightOutput, 1.0f) * texture(imageRocks, FragTexCoords);
	}
	// Blended Rocks/Stone and Snow

	// Snow
	else
	{
		// Calculate the final color
		FinalColor = vec4(LightOutput, 1.0f) * texture(imageSnow, FragTexCoords);
	}
}