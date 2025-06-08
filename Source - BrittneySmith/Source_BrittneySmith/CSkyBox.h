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
// Additional includes
#include <string>
#include "CCamera.h"
#include <stb_image.h>
#include "CShaderLoader.h"


class CSkyBox
{
private:
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;

	std::string m_cubemapFilePathArray[6] {};

	void SetFilePaths();
	void LoadTextures();

	CCamera* m_pCamera = nullptr;
	glm::mat4 m_objModelMat;
	glm::mat4 m_PVMMatrix;

public:
	GLuint m_skyBoxTexture;

	CSkyBox(CCamera* _Camera);
	~CSkyBox() {};
	void Update(float _deltaTIme);
	void Render(GLuint _program, CCamera* _Camera);
}; 

