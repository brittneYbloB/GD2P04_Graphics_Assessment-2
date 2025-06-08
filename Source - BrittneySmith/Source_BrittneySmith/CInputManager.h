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

#include <gtc/type_ptr.hpp>
#include <iostream>

#include "CCamera.h"
#include "CTimeManager.h"
#include "CLightManager.h"

class CInputManager
{
private:
	CCamera* m_pCamera = nullptr;
	CTimeManager* m_pTimeManager = nullptr;
	CLightManager* m_pLightManager = nullptr;
	bool m_bWireFramesEnabled = false;
	bool m_bCullingEnabled = true;
	bool m_bMSAAEnabled = false;
	int m_postProcessingNum = 0;

public:
	CInputManager(CCamera* _Camera, CTimeManager* _TimeManager, CLightManager* _LightManager, class CSceneManager* _SceneManager);
	~CInputManager();

	void SetCallbacks(GLFWwindow* _InputWindow);
	void TextCallbackInput(GLFWwindow* _InputWindow);
	void KeyInput(GLFWwindow* _InputWindow, int _Key, int _ScanCode, int _Action, int _Mods);
	void TextInput(GLFWwindow* _InputWindow, unsigned int _CodePoint);
	void MouseButtonInput(GLFWwindow* _InputWindow, int _Button, int _Action, int _Mods);
	void CursorPositionInput(GLFWwindow* _InputWindow, double _XPos, double _YPos);
	void ScrollBack(GLFWwindow* _InputWindow, double _XPos, double _YPos);

public:
	class CSceneManager* m_pSceneManager = nullptr;

};

