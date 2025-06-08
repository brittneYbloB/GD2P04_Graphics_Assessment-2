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

#include "CModelMesh.h"
#include "CTextureLoader.h"
#include "CTexture3D.h"

#include <iostream>
#include <string>
#include <vector>



// class
class CModel : public CModelMesh
{
public:
	CModel() {};
	~CModel() {};

	void Mesh_Model(std::string _FolderPath, std::string _FileName);

	CTexture3D* m_p3DTexture = nullptr;
};

