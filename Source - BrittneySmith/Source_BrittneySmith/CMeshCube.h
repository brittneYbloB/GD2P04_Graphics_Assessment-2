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
// Other libraries
#include <iostream>
#include <vector>
#include "CCamera.h"
#include "CObjectBase3D.h"

class CMeshCube
	: public CObjectBase3D
{
public:
	CMeshCube();
	~CMeshCube() {};

	void GenerateMesh();
	void SetVertexAttributes();
	void UpdateObject(float _rotationAngle, glm::vec3 _axisRotation, float _deltaTime);
	void Render(GLuint _Program, CCamera* _Camera, CLightManager* _lightManager);
};