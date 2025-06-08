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

class CFramebuffer
{
public:
	CFramebuffer();
	~CFramebuffer();

	void RenderBufferObject();

	GLuint m_FBO;
	GLuint m_renderTexture;

private:
	GLuint m_RBO;

	void TextureBinding();
	void CleanUp();
};

