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

#include "CLightManager.h"
#include <gtc/type_ptr.hpp>


/***************************************************************************
* CLightManager: Constructor which sets up light properties
* @parameters: 
* @return:
****************************************************************************/
CLightManager::CLightManager()
{
	SetGeneralLightProperties();

	SetMultipleLights(0, glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.5f, 0.7f), 0.03f, 1.0f, 1.0f, 0.045f, 0.0075f);
	SetMultipleLights(1, glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.03f, 1.0f, 1.0f, 0.022f, 0.0019f);
	
	SetDirectionalLight(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f), 0.55f, 1.0f);
}


/***************************************************************************
* SetGeneralLightProperties: Sets general lighting variables/properties
* @parameters:
* @return:
****************************************************************************/
void CLightManager::SetGeneralLightProperties()
{
	// Set up lighting variables
	m_fAmbientStrength = 0.05f;
	m_AmbientColor = { 1.0f, 1.0f, 1.0f };
	m_LightColor = { 1.0f, 1.0f, 1.0f };
	m_LightPos = { 2.0f, 6.0f, 3.0f };
	m_fLightSpecularStrength = 1.0f;
	m_fShininess = 32.0f;
	// Rim properties
	m_fRimExponent = 2.0f;
	m_RimColor = { 0.0f, 1.0f, 0.0f };
}


/***************************************************************************
* SetMultipleLights: Set the multiple lights' struct values
* @parameters: Light's: position, colour, ambient strength, specular strength,
* attenuation const, attenuation linear, att exponential
* @return:
****************************************************************************/
void CLightManager::SetMultipleLights(
	int _i,
	glm::vec3 _position,
	glm::vec3 _color,
	float _ambientStrength,
	float _specularStrength,
	float _attConst,
	float _attLinear,
	float _attExp)
{
	PointLights[_i].m_fAmbientStrength = _ambientStrength;
	PointLights[_i].m_color = _color;
	PointLights[_i].m_position = _position;
	PointLights[_i].m_fSpecularStrength = _specularStrength;

	PointLights[_i].m_fAttenuationConstant = _attConst;
	PointLights[_i].m_fAttenuationLinear = _attLinear;
	PointLights[_i].m_fAttenuationExponent = _attExp;
}


/***************************************************************************
* SetDirectionalLight: Set the direction lights' struct values
* @parameters: Light's: direction, colour, ambient strength, specular strength
* @return:
****************************************************************************/
void CLightManager::SetDirectionalLight(glm::vec3 _direction, glm::vec3 _color, float _ambientStrenght, float _specularStrength)
{
	DirLight.m_direction = _direction;
	DirLight.m_color = _color;
	DirLight.m_fAmbientStrength = _ambientStrenght;
	DirLight.m_fSpecularStrength = _specularStrength;

}


/***************************************************************************
* LightingShader: Pass lighting variables into shader via uniform
* @parameters: Program in use
* @return:
****************************************************************************/
void CLightManager::LightingShader(GLuint _Program)
{
	// Toggling the lights ON/OFF
	glUniform1i(glGetUniformLocation(_Program, "bIsDirectionalLightOn"), m_bDirectionalLightOn);
	glUniform1i(glGetUniformLocation(_Program, "bArePointLightsLightOn"), m_bPointLightsOn);

	// General lighting variables to pass in via uniform
	glUniform3fv(glGetUniformLocation(_Program, "AmbientColor"), 1, glm::value_ptr(m_AmbientColor));
	glUniform3fv(glGetUniformLocation(_Program, "LightColor"), 1, glm::value_ptr(m_LightColor));
	glUniform3fv(glGetUniformLocation(_Program, "LightPos"), 1, glm::value_ptr(m_LightPos));
	glUniform3fv(glGetUniformLocation(_Program, "RimColor"), 1, glm::value_ptr(m_RimColor));

	glUniform1f(glGetUniformLocation(_Program, "AmbientStrength"), m_fAmbientStrength);
	glUniform1f(glGetUniformLocation(_Program, "LightSpecularStrength"), m_fLightSpecularStrength);
	glUniform1f(glGetUniformLocation(_Program, "Shininess"), m_fShininess);
	glUniform1f(glGetUniformLocation(_Program, "RimExponent"), m_fRimExponent);


	///* POINT LIGHTS */
	if (m_bPointLightsOn)
	{
		for (int i = 0; i < MAX_POINT_LIGHTS; i++)
		{
			if (i == 0)
			{
				glUniform3fv(glGetUniformLocation(_Program, "PointLights[0].LightPos"), 1, glm::value_ptr(PointLights[0].m_position));
				glUniform3fv(glGetUniformLocation(_Program, "PointLights[0].LightColor"), 1, glm::value_ptr(PointLights[0].m_color));
				glUniform1f(glGetUniformLocation(_Program, "PointLights[0].AmbientStrength"), PointLights[0].m_fAmbientStrength);
				glUniform1f(glGetUniformLocation(_Program, "PointLights[0].LightSpecularStrength"), PointLights[0].m_fSpecularStrength);

				glUniform1f(glGetUniformLocation(_Program, "PointLights[0].AttenuationConstant"), PointLights[0].m_fAttenuationConstant);
				glUniform1f(glGetUniformLocation(_Program, "PointLights[0].AttenuationLinear"), PointLights[0].m_fAttenuationLinear);
				glUniform1f(glGetUniformLocation(_Program, "PointLights[0].AttenuationExponent"), PointLights[0].m_fAttenuationExponent);
			}
			if (i == 1)
			{
				glUniform3fv(glGetUniformLocation(_Program, "PointLights[1].LightPos"), 1, glm::value_ptr(PointLights[1].m_position));
				glUniform3fv(glGetUniformLocation(_Program, "PointLights[1].LightColor"), 1, glm::value_ptr(PointLights[1].m_color));
				glUniform1f(glGetUniformLocation(_Program, "PointLights[1].AmbientStrength"), PointLights[1].m_fAmbientStrength);
				glUniform1f(glGetUniformLocation(_Program, "PointLights[1].LightSpecularStrength"), PointLights[1].m_fSpecularStrength);

				glUniform1f(glGetUniformLocation(_Program, "PointLights[1].AttenuationConstant"), PointLights[1].m_fAttenuationConstant);
				glUniform1f(glGetUniformLocation(_Program, "PointLights[1].AttenuationLinear"), PointLights[1].m_fAttenuationLinear);
				glUniform1f(glGetUniformLocation(_Program, "PointLights[1].AttenuationExponent"), PointLights[1].m_fAttenuationExponent);
			}
		}
	}

	/* DIRECTIONAL LIGHT */
	glUniform3fv(glGetUniformLocation(_Program, "dirLight.Direction"), 1, glm::value_ptr(DirLight.m_direction));
	glUniform3fv(glGetUniformLocation(_Program, "dirLight.Color"), 1, glm::value_ptr(DirLight.m_color));
	glUniform1f(glGetUniformLocation(_Program, "dirLight.AmbientStrength"), DirLight.m_fAmbientStrength);
	glUniform1f(glGetUniformLocation(_Program, "dirLight.SpecularStrength"), DirLight.m_fSpecularStrength);

}