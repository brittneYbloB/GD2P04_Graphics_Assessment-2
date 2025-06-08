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

#include "CTimeManager.h"

/***************************************************************************
* GetDeltaTime: Calculate delta time by calculating the difference between
*				the previous and currrent time steps/frames
* @parameters:
* @return:		Delta time
****************************************************************************/
void CTimeManager::CalculateDeltaTime()
{
	// Retrieve the current time/second
	m_fCurrentTimeStep = GetCurrentTimeStep();
	// Calculate delta time to be the difference between the current and last frame/time stamp
	m_fDeltaTime = m_fCurrentTimeStep - m_fPreviousTimeStep;
	// Move timesteps according to time
	if (m_fDeltaTime >= 1.0f)
	{
		m_fPreviousTimeStep = m_fCurrentTimeStep;
		m_fDeltaTime = 0.0f;
	}
}


/***************************************************************************
* GetCurrentTimeStep: Get current time using glfwGetTime()
* @parameters:
* @return: CurrentTimeStep
****************************************************************************/
float CTimeManager::GetCurrentTimeStep()
{
	m_fCurrentTimeStep = (float)glfwGetTime();

	return m_fCurrentTimeStep;
}


/***************************************************************************
* CalculateFrames: Calculate delta time AND use to determine/move animation frame
* @parameters:
* @return:
****************************************************************************/
void CTimeManager::CalculateFrames()
{
	// Retrieve the current time/second
	m_fCurrentTimeStep = GetCurrentTimeStep();
	// Calculate delta time to be the difference between the current and last frame/time stamp
	m_fDeltaTime = m_fCurrentTimeStep - m_fPreviousTimeStep;
	// Move frames according to time
	if (m_fDeltaTime >= 1.0f / frame_pos)
	{
		m_fPreviousTimeStep = m_fCurrentTimeStep;
		m_fDeltaTime = 0.0f;

		// Frame positioning
		m_UV_X += 1.0f;
		if (m_UV_X >= m_NX_Frames)
		{
			m_UV_X = 0.0f;
		}
	}
}
