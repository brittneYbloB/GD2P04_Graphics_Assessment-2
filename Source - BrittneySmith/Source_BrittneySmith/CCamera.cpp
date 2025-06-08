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

#include "CCamera.h"

/***************************************************************************
* Camera: Initialise camera variables
* @parameters:
* @return:
****************************************************************************/
CCamera::CCamera()
{
	// Set movement to be initially towards the right
	m_bMoveRight = true;

	// Setup camera size
	m_fWindowWidth = 800.0f;
	m_fWindowHeight = 800.0f;

	m_fHalfWindowWidth = (float)m_fWindowWidth * 0.5f;
	m_fHalfWindowHeight = (float)m_fWindowHeight * 0.5f;

	// Setup the camera's position, target position, look directions and up direction
	m_cameraPos = SetCameraPos();
	m_cameraTargetPos = SetCameraTargetPos();
	m_cameraLookDir = SetCameraLookDir();
	m_cameraUpDir = SetCameraUpDir();
}


/***************************************************************************
* SetCameraPos: Initialise/declare camera position
* @return: glm::vec3 coordinates
****************************************************************************/
glm::vec3 CCamera::SetCameraPos()
{
	return glm::vec3(0.0f, 0.0f, 3.0f);
}


/***************************************************************************
* SetCameraTargetPos: Initialise/declare camera's target's position
* @return: glm::vec3 coordinates
****************************************************************************/
glm::vec3 CCamera::SetCameraTargetPos()
{
	return glm::vec3(0.0f, 0.0f, 0.0f);
}


/***************************************************************************
* SetCameraLookDir: Initialise/declare camera's facing/looking direction
* @return: glm::vec3 coordinates (down the -Z axis)
****************************************************************************/
glm::vec3 CCamera::SetCameraLookDir()
{
	return glm::vec3(0.0f, 0.0f, -1.0f);
}


/***************************************************************************
* SetCameraUpDir: Initialise/declare camera's upwards direction
* @return: glm::vec3 coordinates (down the +Y axis)
****************************************************************************/
glm::vec3 CCamera::SetCameraUpDir()
{
	return glm::vec3(0.0f, 1.0f, 0.0f);
}


/***************************************************************************
* CalculateProjectionMatrix: Calculate the projection matrix
* @return:
****************************************************************************/
void CCamera::CalculateProjectionMatrix()
{
	// Orthographic Projection matrix - Anchor point (0, 0) at the centre]
	//ProjectionMat = glm::ortho(-1 * (HalfWindowWidth), HalfWindowWidth, -1 * (HalfWindowHeight), HalfWindowHeight, 0.1f, 100.0f);
	m_projectionMat = glm::perspective(glm::radians(m_fFieldOfView), (float)m_fWindowWidth / (float)m_fWindowHeight, 0.1f, 4000.0f);
}


/***************************************************************************
* CalculateViewMatrix: Calculate the view matrix
* @return:
****************************************************************************/
void CCamera::CalculateViewMatrix()
{
	// Camera view matrix
	m_viewMat = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUpDir);
}


/***************************************************************************
* CameraUpdate: Update the camera's position and target position to simulate 
				movement
* @parameters:  TimeManager pointer created in main function to access
*				time functions/variables for movement
* @return:
****************************************************************************/
void CCamera::CameraUpdate(CTimeManager* _pTimeManager)
{
	// Moving the camera to the right
	if (m_cameraPos.x >= 100.0f)
	{
		m_bMoveRight = false;
	}
	if (m_cameraPos.x <= -100.0f)
	{
		m_bMoveRight = true;
	}
	if (m_bMoveRight)
	{
		m_cameraPos += glm::vec3(5.0f, 0.0f, 0.0f) * _pTimeManager->m_fDeltaTime / 60.0f;
		m_cameraTargetPos += glm::vec3(5.0f, 0.0f, 0.0f) * _pTimeManager->m_fDeltaTime / 60.0f;
	}
	if (!m_bMoveRight)
	{
		m_cameraPos -= glm::vec3(5.0f, 0.0f, 0.0f) * _pTimeManager->m_fDeltaTime / 60.0f;
		m_cameraTargetPos -= glm::vec3(5.0f, 0.0f, 0.0f) * _pTimeManager->m_fDeltaTime / 60.0f;
	}
}


/***************************************************************************
* CameraMovement: Camera movement (Pitch & Yaw)
* @parameters:  Window, mouse X and Y positions
* @return:
****************************************************************************/
void CCamera::CameraMovement(GLFWwindow* _InputWindow, double _XPos, double _YPos)
{
	// Prevent a large, sudden jump whenever the window first receives focus of the mouse cursor
	if (m_bIsFirstMousePos) // initially set to true
	{
		m_fLastPos_X = (float)_XPos;
		m_fLastPos_Y = (float)_YPos;
		m_bIsFirstMousePos = false;
	}

	// 1. Calculate the mouse's offset since the last frame.
	float fOffset_X = (float)_XPos - m_fLastPos_X;
	float fOffset_Y = m_fLastPos_Y - (float)_YPos; // reversed since y-coordinates range from bottom to top
	m_fLastPos_X = (float)_XPos;
	m_fLastPos_Y = (float)_YPos;

	fOffset_X *= m_fCameraMovementSensitivity;
	fOffset_Y *= m_fCameraMovementSensitivity;

	// 2. Add the offset values to the camera's yaw and pitch values.
	m_fYaw += fOffset_X;
	m_fPitch += fOffset_Y;

	// 3. Add some constraints to the minimum / maximum pitch values.
	if (m_fPitch > 89.0f)
	{
		m_fPitch = 89.0f;
	}
	if (m_fPitch < -89.0f)
	{
		m_fPitch = -89.0f;
	}

	// 4. Calculate the direction vector.
	glm::vec3 m_direction;
	m_direction.x = cos(glm::radians(m_fYaw)) * cos(glm::radians(m_fPitch));
	m_direction.y = sin(glm::radians(m_fPitch));
	m_direction.z = sin(glm::radians(m_fYaw)) * cos(glm::radians(m_fPitch));
	m_cameraFront = glm::normalize(m_direction);
}