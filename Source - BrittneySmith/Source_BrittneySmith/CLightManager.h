/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) [2022] Media Design School
File Name :
Description : GD2P04 : 3D Graphics Programming
Author : Brittney Smith
Mail : Brittney.Smith@mds.ac.nz
**************************************************************************/

#pragma once
// Library Includes
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

struct PointLight
{
	glm::vec3 m_position;
	glm::vec3 m_color;
	float m_fAmbientStrength;
	float m_fSpecularStrength;

	float m_fAttenuationConstant;
	float m_fAttenuationLinear;
	float m_fAttenuationExponent;
};

struct DirLight
{
	glm::vec3 m_direction;
	glm::vec3 m_color;
	float m_fAmbientStrength;
	float m_fSpecularStrength;
};



class CLightManager
{
private:
	float m_fAmbientStrength = 0.0f;
	glm::vec3 m_AmbientColor = {0.0f, 0.0f, 0.0f};
	glm::vec3 m_LightColor = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_LightPos = { 0.0f, 0.0f, 0.0f };
	float m_fLightSpecularStrength = 0.0f;
	float m_fShininess = 0.0f;
	float m_fRimExponent = 0.0f;
	glm::vec3 m_RimColor = { 0.0f, 0.0f, 0.0f };

public:
	bool m_bPointLightsOn = true;
	bool m_bDirectionalLightOn = true;

	static const int MAX_POINT_LIGHTS = 4;
	PointLight PointLights[MAX_POINT_LIGHTS];
	DirLight DirLight;

	CLightManager();
	~CLightManager() {};

	void SetGeneralLightProperties();
	void SetMultipleLights(int _i, glm::vec3 _position, glm::vec3 _color, float _ambientStrength, float _specularStrength, float _attConst, float _attLinear, float _attExp);
	void SetDirectionalLight(glm::vec3 _direction, glm::vec3 _color, float _ambientStrenght, float _specularStrength);
	void LightingShader(GLuint _Program);
};