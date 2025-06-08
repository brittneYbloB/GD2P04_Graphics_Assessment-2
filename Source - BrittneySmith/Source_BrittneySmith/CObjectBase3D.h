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
// Classes
#include "CTimeManager.h"
#include "CCamera.h"
#include "CLightManager.h"


struct VertexPoint
{
public:
	glm::vec3 Position;
	glm::vec2 TextureCoord;

	VertexPoint() {};
	VertexPoint(glm::vec3 _pos, glm::vec2 _textureCoord)
	{
		Position = _pos;
		TextureCoord = _textureCoord;
	}

	bool operator == (const VertexPoint& _other) const
	{
		return (Position == _other.Position); // && TextureCoord = _other.TextureCoord
	}
};

class CObjectBase3D
{
protected:

	CTimeManager* m_pTimeManager = nullptr;						// Time manager reference

	GLuint m_VAO = 0;											// VAO	
	GLuint m_VBO = 0;											// VBO	
	GLuint m_EBO = 0;											// EBO	
	GLuint m_texture = 0;										// Texture

	glm::vec3 m_objScale2D = glm::vec3(1.0f, 1.0f, 1.0f);		// Scalar
	glm::vec3 m_objScale3D = glm::vec3(0.4f, 0.4f, 0.4f);		// Scalar
	float m_fObjRotationAngle = 0.0f;							// Rotation angle

	// Transformation matrices
	glm::mat4 m_TranslationMat;									// Translation matrix
	glm::mat4 m_ScaleMat2D;										// Scale matrices
	glm::mat4 m_ScaleMat3D;										// Scale matrices
	glm::mat4 m_RotationMat;									// Rotation Matrix
	glm::mat4 m_objModelMat;									// UNIQUE model matrices for each object

	// PVM
	glm::mat4 m_PVMMatrix;
	GLint m_PVMMatrixLoc;

	// Condition booleans
	bool m_bIsReflective = false;								// Reflective status boolean


protected:

	// Functions to override by child classes
	virtual void ObjectSetup(GLuint _Texture) {};	// do not intend to ovveride this
	virtual void UpdateObject(float _rotationAngle, glm::vec3 _axisRotation, float _deltaTime) {};
	virtual void RenderObject(GLuint _Program, CCamera* _Camera) {};
	virtual void StencilRender(GLuint _Program, CCamera* _Camera, CLightManager* _lightManager) {};

	// Constructor & Destructor
	CObjectBase3D() { m_texture = 0; m_PVMMatrixLoc = 0; m_PVMMatrix = {}; delete m_pTimeManager; };
	virtual ~CObjectBase3D() {};


public:
	glm::vec3 m_objPosition = {};								// World Position (change for each child instance)
};