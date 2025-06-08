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
class CQuadFromTrianglePatch :
    public CObjectBase3D
{
public:
	CQuadFromTrianglePatch();
	~CQuadFromTrianglePatch();

	void UpdateObject(float _rotationAngle, glm::vec3 _axisRotation, float _deltaTime);
	void Render(GLuint _Program, CCamera* _Camera, CLightManager* _lightManager);

protected:

	int m_drawCount;
	GLenum m_drawType;
	std::vector<VertexPoint> Vertices;
	std::vector<glm::vec2> Texture;

	void GenerateMesh();

private:

	GLfloat Vertices_Quad[30] = {
		// Index	 Position				Texture Coordinates
		/* 1 */		-1.0f,  1.0f, 0.0f,		0.0f, 1.0f, // Top Left
		/* 2 */		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f, // Bot Left
		/* 3 */		 1.0f, -1.0f, 0.0f,		1.0f, 0.0f, // Bot Right

		/* 1 */		-1.0f,  1.0f, 0.0f,		0.0f, 1.0f, // Top Left
		/* 2 */		 1.0f, -1.0f, 0.0f,		1.0f, 0.0f, // Bot Right
		/* 3 */		 1.0f,  1.0f, 0.0f,		1.0f, 1.0f, // Top Right
	};

	// Index/Element Array
	GLuint Indices_Quad[6] = {
		0, 1, 2,	// First Triangle (TL -> BL -> BR)
		0, 2, 3,	// Second Triangle (TL -> BR -> TR)
	};
};

