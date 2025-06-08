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
#include <tiny_obj_loader.h>

// Math Libraries
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
// Other libraries
#include <iostream>
#include <vector>
#include "CCamera.h"
#include "CLightManager.h"

// Vertex format structure for convenience
struct VertexFormat
{
public:
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec3 Normal;

	VertexFormat() {};
	VertexFormat(glm::vec3 _Pos, glm::vec2 _TexCoord, glm::vec3 _Norm)
	{
		Position = _Pos;
		TexCoord = _TexCoord;
		Normal = _Norm;
	}
};

class CModelMesh
{
private:
	// VBO & EBO
	GLuint m_VBO = 0;
	//GLuint m_EBO = 0;

	glm::mat4 m_ScaleMat2D;
	glm::mat4 m_ScaleMat3D;

	// Matrices for transformations
	glm::mat4 m_TranslationMat;

	// Rotation & Scaling
	float m_fObjRotationAngle = 0.0f;
	glm::mat4 m_RotationMat;

	// PVM
	glm::mat4 m_PVMMatrix;
	GLint m_PVMMatrixLoc;

public:

	bool m_is3DTextured = false;

	// World Position (change for each child instance)
	glm::vec3 m_objPosition = { 0.25f, 0.0f, 1.0f };
	// VAO (public for vertex array binding in render function)
	GLuint m_VAO = 0;
	std::vector<VertexFormat> m_Vertices;
	GLuint m_texture;
	// Scale
	glm::vec3 m_objScale2D = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 m_objScale3D = glm::vec3(1.5f, 1.5f, 1.5f);
	// UNIQUE model matrices for each object
	glm::mat4 m_objModelMat;


	CModelMesh() {};
	~CModelMesh() {};

	void GenerateMesh();
	void SetVertexAttributes();
	void UpdateObject(float _rotationAngle, glm::vec3 _axisRotation, float _deltaTime);
	void Render(GLuint _Program, CCamera* _Camera, CLightManager* _lightManager);
};

