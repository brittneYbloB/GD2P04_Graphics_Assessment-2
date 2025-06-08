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

#include "CInputManager.h"
#include "CSceneManager.h"

/***************************************************************************
* CInputManager: Constructor to set some private variables
* @parameters: Pointer: Camera, time manager, light manager
* @return:
****************************************************************************/
CInputManager::CInputManager(CCamera* _Camera, CTimeManager* _TimeManager, CLightManager* _LightManager, CSceneManager* _SceneManager)
{
	m_pCamera = _Camera;
	m_pTimeManager = _TimeManager;
	m_pLightManager = _LightManager;
	m_pSceneManager = _SceneManager;
}

/***************************************************************************
* ~CInputManager: Destructor; clean up pointers
* @parameters:
* @return:
****************************************************************************/
CInputManager::~CInputManager()
{
	m_pCamera = nullptr;
	m_pTimeManager = nullptr;
	m_pLightManager = nullptr;
}


/***************************************************************************
* SetCallbacks: Set the callbacks; called in the initial setup from main
* @parameters:
* @return:
****************************************************************************/
void CInputManager::SetCallbacks(GLFWwindow* _InputWindow)
{
	// Cast the data to an instance of this class
	CInputManager* pInput = static_cast<CInputManager*>(glfwGetWindowUserPointer(_InputWindow));

	// Set window user pointer
	glfwSetWindowUserPointer(_InputWindow, this);
	// Static cast to function
	auto keyCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods) 
	{
		static_cast<CInputManager*>(glfwGetWindowUserPointer(window))->KeyInput(window, key, scancode, action, mods);
	};
	glfwSetKeyCallback(_InputWindow, keyCallback);

	// Set the callback function for input processing
	//glfwSetKeyCallback(_InputWindow, KeyInput);

	// Set the callback function for mouse button input
	auto mouseButtonInput = [](GLFWwindow* window, int button, int action, int mods) 
	{
		static_cast<CInputManager*>(glfwGetWindowUserPointer(window))->MouseButtonInput(window, button, action, mods);
	};
	glfwSetMouseButtonCallback(_InputWindow, mouseButtonInput);

	// Raw mouse motion
	glfwSetInputMode(_InputWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
	{
		glfwSetInputMode(_InputWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}

	// Set the callback function fo the cursor position
	auto cursorPositionInput = [](GLFWwindow* window, double _XPos, double _YPos) 
	{
		static_cast<CInputManager*>(glfwGetWindowUserPointer(window))->CursorPositionInput(window, _XPos, _YPos);
	};
	glfwSetCursorPosCallback(_InputWindow, cursorPositionInput);

	// Zooming in/out
	auto scrollBack = [](GLFWwindow* window, double _XPos, double _YPos) 
	{
		static_cast<CInputManager*>(glfwGetWindowUserPointer(window))->ScrollBack(window, _XPos, _YPos);
	};
	glfwSetScrollCallback(_InputWindow, scrollBack);
}


/***************************************************************************
* TextCallbackInput: Text input callback; called in Update() in main cpp
* @parameters:
* @return:
****************************************************************************/
void CInputManager::TextCallbackInput(GLFWwindow* _InputWindow)
{
	// Set the callback function for text input
	auto textInput = [](GLFWwindow* window, unsigned int codePoint) 
	{
		static_cast<CInputManager*>(glfwGetWindowUserPointer(window))->TextInput(window, codePoint);
	};
	(true) ? glfwSetCharCallback(_InputWindow, textInput) : glfwSetCharCallback(_InputWindow, 0);
}

/***************************************************************************
* KeyInput: Callback function called in response to keyboard input. Processed during glfwPollEvents().
* @parameters: Input window, key, scan code, action, mods
* @return:
****************************************************************************/
void CInputManager::KeyInput(GLFWwindow* _InputWindow, int _Key, int _ScanCode, int _Action, int _Mods)
{
	// Exit application
	if (_Key == GLFW_KEY_ESCAPE && _Action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(_InputWindow, true);
	}

	// Load Scene 1
	else if (glfwGetKey(_InputWindow, GLFW_KEY_1) == GLFW_PRESS)
	{
		m_pSceneManager->m_sceneSelector = SCENE::e_SCENE_1;
		
	}

	// Load Scene 2
	else if (glfwGetKey(_InputWindow, GLFW_KEY_2) == GLFW_PRESS)
	{
		m_pSceneManager->m_sceneSelector = SCENE::e_SCENE_2;
		
	}

	// Load Scene 3
	else if (glfwGetKey(_InputWindow, GLFW_KEY_3) == GLFW_PRESS)
	{
		m_pSceneManager->m_sceneSelector = SCENE::e_SCENE_3;
	}

	// Switch post-processing effects
	else if (glfwGetKey(_InputWindow, GLFW_KEY_TAB) == GLFW_PRESS)
	{
		if (m_postProcessingNum + 1 >= 5) { m_postProcessingNum = 0; }
		else { m_postProcessingNum += 1; }

		switch (m_postProcessingNum)
		{
			case 0:
			{
				m_pSceneManager->m_postProccessingEffect = POSTPROCESSING::e_NONE;
				break;
			}
			case 1:
			{
				m_pSceneManager->m_postProccessingEffect = POSTPROCESSING::e_COLORINVERSION;
				break;
			}
			case 2:
			{
				m_pSceneManager->m_postProccessingEffect = POSTPROCESSING::e_GREYSCALE;
				break;
			}
			case 3:
			{
				m_pSceneManager->m_postProccessingEffect = POSTPROCESSING::e_RAIN;
				break;
			}
			case 4:
			{
				m_pSceneManager->m_postProccessingEffect = POSTPROCESSING::e_TVCRT;
				break;
			}

			default:
				break;
		}
	}
}


/***************************************************************************
* TextInput: Callback function called in response to keyboard input for text printing.
Processed during glfwPollEvents().
* @parameters: Input window, code point
* @return:
****************************************************************************/
void CInputManager::TextInput(GLFWwindow* _InputWindow, unsigned int _CodePoint)
{
	// Forward
	if (glfwGetKey(_InputWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		//m_pCamera->m_cameraPos += glm::vec3(0.0f, 0.0f, -0.1f) * m_pTimeManager->m_fDeltaTime;
		m_pCamera->m_cameraPos += m_pCamera->m_cameraFront * 0.064f;
	}
	// Backward (+Z)												 
	if (glfwGetKey(_InputWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_pCamera->m_cameraPos -= m_pCamera->m_cameraFront * 0.064f;
	}
	// Left (-X)													   
	if (glfwGetKey(_InputWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		//m_pCamera->m_cameraTargetPos += glm::vec3(-0.1f, 0.0f, 0.0f) * 0.64f;
		//m_pCamera->m_cameraPos += glm::vec3(-0.1f, 0.0f, 0.0f) * 0.64f;

		m_pCamera->m_cameraPos -= glm::normalize(glm::cross(m_pCamera->m_cameraFront, m_pCamera->m_cameraUpDir)) * 0.064f;
	}
	// Right (+X)													
	if (glfwGetKey(_InputWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		//m_pCamera->m_cameraTargetPos += glm::vec3(0.1f, 0.0f, 0.0f) * 0.64f;
		//m_pCamera->m_cameraPos += glm::vec3(0.1f, 0.0f, 0.0f) * 0.64f;

		m_pCamera->m_cameraPos += glm::normalize(glm::cross(m_pCamera->m_cameraFront, m_pCamera->m_cameraUpDir)) * 0.064f;
	}
	// Move Up (+Y)														
	if (glfwGetKey(_InputWindow, GLFW_KEY_Q) == GLFW_PRESS)
	{
		m_pCamera->m_cameraTargetPos += glm::vec3(0.0f, 0.1f, 0.0f) * 0.64f;
		m_pCamera->m_cameraPos += glm::vec3(0.0f, 0.1f, 0.0f) * 0.64f;
	}
	// Move Down (-Y)													
	if (glfwGetKey(_InputWindow, GLFW_KEY_E) == GLFW_PRESS)
	{
		m_pCamera->m_cameraTargetPos += glm::vec3(0.0f, -0.1f, 0.0f) * 0.64f;
		m_pCamera->m_cameraPos += glm::vec3(0.0f, -0.1f, 0.0f) * 0.64f;
	}

	// Enable / Disable Wireframes
	if (glfwGetKey(_InputWindow, GLFW_KEY_X) == GLFW_PRESS)
	{
		// Wireframes visible
		if (m_bWireFramesEnabled)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			m_bWireFramesEnabled = false;
		}
		else // Disable wireframes
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			m_bWireFramesEnabled = true;
		}
	}

	// Enable / Disable Culling
	if (glfwGetKey(_InputWindow, GLFW_KEY_C) == GLFW_PRESS)
	{
		// Wireframes visible
		if (m_bCullingEnabled)
		{
			glDisable(GL_CULL_FACE);
			m_bCullingEnabled = false;
		}
		else // Disable wireframes
		{
			glEnable(GL_CULL_FACE);
			m_bCullingEnabled = true;
		}
	}

	// Enable / Disable MSAA
	if (glfwGetKey(_InputWindow, GLFW_KEY_V) == GLFW_PRESS)
	{
		// Wireframes visible
		if (m_bMSAAEnabled)
		{
			glEnable(GL_MULTISAMPLE);
			m_bMSAAEnabled = false;
		}
		else // Disable wireframes
		{
			glDisable(GL_MULTISAMPLE);
			m_bMSAAEnabled = true;
		}
	}

}


/***************************************************************************
* MouseButtonInput: Callback function called in response to mouse button input.
Processed during glfwPollEvents().
* @parameters: Input window, button, action, mods
* @return:
****************************************************************************/
void CInputManager::MouseButtonInput(GLFWwindow* _InputWindow, int _Button, int _Action, int _Mods)
{
	if (_Button == GLFW_MOUSE_BUTTON_RIGHT && _Action == GLFW_PRESS)
	{
		std::cout << "Mouse Button Right: Press" << std::endl;
	}
	if (_Button == GLFW_MOUSE_BUTTON_RIGHT && _Action == GLFW_RELEASE)
	{
		std::cout << "Mouse Button Right: Release" << std::endl;
	}
}


/***************************************************************************
* CursorPositionInput: Callback function called in response to cursor movement within
the bounds of the window context. Process during glfwPollEvents().
* @parameters: Input window, mouse X position, mouse Y position
* @return:
****************************************************************************/
void CInputManager::CursorPositionInput(GLFWwindow* _InputWindow, double _XPos, double _YPos)
{
	m_pCamera->CameraMovement(_InputWindow, _XPos, _YPos);
}

/***************************************************************************
* ScrollBack: Increase or decrease camera's FOV based on mouse-scroll
* @parameters: Input window, mouse X position, mouse Y position
* @return:
****************************************************************************/
void CInputManager::ScrollBack(GLFWwindow* _InputWindow, double _XPos, double _YPos)
{
	m_pCamera->m_fFieldOfView -= (float)_YPos;
	if (m_pCamera->m_fFieldOfView < 1.0f)
		m_pCamera->m_fFieldOfView = 1.0f;
	if (m_pCamera->m_fFieldOfView > 45.0f)
		m_pCamera->m_fFieldOfView = 45.0f;
}