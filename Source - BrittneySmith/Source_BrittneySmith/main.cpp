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

// Library Defines & Includes
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
// Math Libraries
#include <gtc/matrix_transform.hpp>
// Classes
#include "CSceneManager.h"


/***************************************************************************
* main: Main program (initialise set-up, and run update and render functions)
* @parameter:
* @return:
****************************************************************************/
int main()
{
	CSceneManager* sceneManager = new CSceneManager();


	// Initialising GLFW and setting the version to 4.6 with only Core functionality available
	glfwInit(); // This function initializes the GLFW library. This is necessary before the majority of GLFW functions can be used.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);


	// Create an GLFW controlled context window
	sceneManager->m_pWindow = glfwCreateWindow(800, 800, "OpenGL Window", NULL, NULL);
	if (sceneManager->m_pWindow == NULL)
	{
		std::cout << "GLFW failed to initialise properly. Terminating program." << std::endl;
		system("pause");

		glfwTerminate();
		return -1;
		/* Terminate:
		• Destroys all the remaining windows and cursors, freeing allocated resources.
		• glfwInit() must be called again in order to use GLFW functions after this is called.
		*/
	}

	glfwMakeContextCurrent(sceneManager->m_pWindow);
	/*
	• window: The window whose context to make current.
	• Or NULL to detach the current context.
	• Specifies that the given window (context) is available and current on the calling
	thread.
	*/

	// Initializing GLEW to populate OpenGL function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW failed to initialize properly. Terminating program." << std::endl;
		system("pause");

		glfwTerminate();
		return -1;
	}

	// Setup the Initial elements of the program
	sceneManager->InitialSetup();

	sceneManager->m_sceneSelector = SCENE::e_SCENE_1;
	sceneManager->m_postProccessingEffect = POSTPROCESSING::e_NONE;

	// Main loop
	while (glfwWindowShouldClose(sceneManager->m_pWindow) == false)
	{
		// Update time, input, skybox and camera
		sceneManager->DefaultUpdate();

		// Clear buffers (color, depth, and stencil)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


		// Switch between scenes
		switch (sceneManager->m_sceneSelector)
		{
		case SCENE::e_SCENE_1:
		{
			// Update all objects and run the processes
			sceneManager->Update_Scene1();
			// Render all the objects
			sceneManager->Render_Scene1();
			break;
		}
		case SCENE::e_SCENE_2:
		{
			sceneManager->Update_Scene2();
			sceneManager->Render_Scene2();
			break;
		}
		case SCENE::e_SCENE_3:
		{
			sceneManager->Update_Scene3();
			sceneManager->Render_Scene3();
			break;
		}

		default:
			break;
		}

		// Unbind assets to prevent accidental use/modification
		glBindVertexArray(0);
		glUseProgram(0);

		// Swaps the front and back buffers of the specified window.
		glfwSwapBuffers(sceneManager->m_pWindow);
	}

	// Ensuring correct shutdown of GLFW
	delete sceneManager;
	glfwTerminate();
	return 0;
}