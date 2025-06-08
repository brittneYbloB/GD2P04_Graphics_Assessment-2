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

class CTimeManager
{
// Public to allow variable access in main for shader functionalities
public:
	// Variables : Animation
	float m_NX_Frames = 8.0f, m_NY_Frames = 1.0f;
	float m_UV_X = 0.0f, m_UV_Y = 1.0f;
	float frame_pos = 8.0f;
	// Variables : Delta time
	float m_fDeltaTime;
	float m_fCurrentTimeStep;
	float m_fPreviousTimeStep;

	// Functions
	CTimeManager() { m_fDeltaTime = 0.0f; m_fCurrentTimeStep = 0.0f; m_fPreviousTimeStep = 0.0f; };
	~CTimeManager() {};

	void CalculateDeltaTime();
	float GetCurrentTimeStep();
	void CalculateFrames();
};