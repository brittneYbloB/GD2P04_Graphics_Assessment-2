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

class CQuad
{
private:
	GLfloat Vertices_Quad[32] = {
		// Index	 Position				Color					Texture Coordinates
		/* 1 */		-1.0f,  1.0f, 0.0f,		1.0f, 0.0f, 0.0f,		0.0f, 1.0f, // Top Left
		/* 2 */		-1.0f, -1.0f, 0.0f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f, // Bot Left
		/* 3 */		 1.0f, -1.0f, 0.0f,		1.0f, 0.0f, 1.0f,		1.0f, 0.0f, // Bot Right
		/* 4 */		 1.0f,  1.0f, 0.0f,		0.0f, 1.0f, 1.0f,		1.0f, 1.0f, // Top Right
	};

	// Index/Element Array
	GLuint Indices_Quad[6] = {
		0, 1, 2,	// First Triangle (TL -> BL -> BR)
		0, 2, 3,	// Second Triangle (TL -> BR -> TR)
	};


	GLuint m_texture;
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;
	GLuint m_programQuad;

public:
	CQuad();
	~CQuad() {};

	void Render(GLuint _postProcessingProgram, GLuint _framebufferColorAttachmentTexture, float _deltaTime);
};

