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
// Math Libraries
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>

#include "CTimeManager.h"

class CCamera
{
private:
	// Mouse movement properties
	float m_fLastPos_X = 400, m_fLastPos_Y = 400;
	const float m_fCameraMovementSensitivity = 0.1f;
	bool m_bIsFirstMousePos = true;
	float m_fYaw = -90;
	float m_fPitch = 0.0f;

	// Window size
	float m_fWindowWidth;
	float m_fWindowHeight;
	float m_fHalfWindowWidth;
	float m_fHalfWindowHeight;

public:
	/* Store projectionand view matrices ready to be retrieved by objects in rendering */

	// Matrices which help keep the objects relative to each other when rendered
	glm::mat4 m_viewMat;
	glm::mat4 m_projectionMat;

	// Movement
	bool m_bMoveRight;

	// Camera Variables
	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraLookDir;
	glm::vec3 m_cameraTargetPos;
	glm::vec3 m_cameraUpDir; 
	glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	float m_fFieldOfView = 45.0f;

	// Functions
	CCamera();
	~CCamera() {};

	glm::vec3 SetCameraPos();
	glm::vec3 SetCameraLookDir();
	glm::vec3 SetCameraTargetPos();
	glm::vec3 SetCameraUpDir();

	void CameraUpdate(CTimeManager* _pTimeManager);
	void CalculateProjectionMatrix();
	void CalculateViewMatrix();
	void CameraMovement(GLFWwindow* _InputWindow, double _XPos, double _YPos);
};

