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
#include "CObjectBase3D.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

class CSphere :
    public CObjectBase3D
{
private:
	GLuint m_programStencil;

	int m_iIndexCount = 0;
	int m_iDrawType = 0;

	int m_drawCount = 0;
	std::vector<VertexPoint> m_Vertices;

public:
	bool m_bIsRed = false;

	CSphere(float Radius, int Fidelity, GLuint _Texture);
	~CSphere();

	void UpdateObject(float _rotationAngle, glm::vec3 _axisRotation, float _deltaTime);
	void Render(GLuint _Program, CCamera* _Camera, CLightManager* _lightManager);
	void StencilRender(GLuint _Program, CCamera* _Camera, CLightManager* _lightManager) override;
};

