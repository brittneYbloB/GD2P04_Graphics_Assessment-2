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
#include "CObjectBase3D.h"

class CQuadPatchTessellationMesh
	: public CObjectBase3D
{
public:
	CQuadPatchTessellationMesh();
	~CQuadPatchTessellationMesh();

	void UpdateObject(float _rotationAngle, glm::vec3 _axisRotation, float _deltaTime);
	void Render(GLuint _Program, CCamera* _Camera, CLightManager* _lightManager);

protected:

	int m_drawCount;
	GLenum m_drawType;
	std::vector<VertexPoint> Vertices;

	void GenerateMesh();
};

