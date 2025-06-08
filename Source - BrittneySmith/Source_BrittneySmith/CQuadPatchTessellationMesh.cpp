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

#include "CQuadPatchTessellationMesh.h"

/***************************************************************************
* CQuadPatchTessellationMesh: Constructor which generates vertices for quad patches
* @parameters:
* @return:
****************************************************************************/
CQuadPatchTessellationMesh::CQuadPatchTessellationMesh()
{
	// Generate triangles
	Vertices.push_back(VertexPoint(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2()));
	Vertices.push_back(VertexPoint(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2()));
	Vertices.push_back(VertexPoint(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec2()));
	Vertices.push_back(VertexPoint(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec2()));

	glPatchParameteri(GL_PATCH_VERTICES, 4);	// Sets Quad patch

	// IF TCS is not used (as is optional), the values can be set in the application
	/*glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, glm::value_ptr(glm::vec4(4.0f, 4.0f, 4.0f, 4.0f)));
	glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, glm::value_ptr(glm::vec2(4.0f, 4.0f)));*/

	m_drawCount = (unsigned int)Vertices.size();
	m_drawType = GL_PATCHES;

	// Generate VAO, VBO and vertex-attribute pointers
	GenerateMesh();

}


/***************************************************************************
* ~CQuadPatchTessellationMesh: Deconstructor
* @parameters:
* @return:
****************************************************************************/
CQuadPatchTessellationMesh::~CQuadPatchTessellationMesh()
{
}


/***************************************************************************
* GenerateMesh: VAO, VBO, and vertex attributes
* @parameters:
* @return:
****************************************************************************/
void CQuadPatchTessellationMesh::GenerateMesh()
{
	/* VAO */
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	///* VBO */
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_drawCount * 3 * sizeof(float), &Vertices[0], GL_STATIC_DRAW);

	// Set Vertex Attribute Data for Quad
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPoint), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


/***************************************************************************
* UpdateObject: Update
* @parameters: float _rotationAngle, glm::vec3 _axisRotation, float _deltaTime
* @return:
****************************************************************************/
void CQuadPatchTessellationMesh::UpdateObject(float _rotationAngle, glm::vec3 _axisRotation, float _deltaTime)
{
	// Translation matrix
	m_TranslationMat = glm::translate(glm::mat4(), m_objPosition);

	// Rotation matrix
	m_RotationMat = glm::rotate(glm::mat4(), (float)glfwGetTime() * glm::radians(_rotationAngle), _axisRotation);

	// Calculating the Scaling Matrix
	m_ScaleMat2D = glm::scale(glm::mat4(), m_objScale2D);
	m_ScaleMat3D = glm::scale(glm::mat4(), m_objScale3D);

	/* Calculate Model Matrix */
	m_objModelMat = m_TranslationMat * m_RotationMat * m_ScaleMat3D;
}


/***************************************************************************
* Render: Render
* @parameters: GLuint _Program, CCamera* _Camera, CLightManager* _lightManager
* @return:
****************************************************************************/
void CQuadPatchTessellationMesh::Render(GLuint _Program, CCamera* _Camera, CLightManager* _lightManager)
{
	glUseProgram(_Program);
	glBindVertexArray(m_VAO);


	/** PVM **/
	m_PVMMatrix = _Camera->m_projectionMat * _Camera->m_viewMat * m_objModelMat;
	m_PVMMatrixLoc = glGetUniformLocation(_Program, "PVMMatrix");
	glUniformMatrix4fv(m_PVMMatrixLoc, 1, GL_FALSE, glm::value_ptr(m_PVMMatrix));
	// Pass in model matrix for lighting
	m_PVMMatrixLoc = glGetUniformLocation(_Program, "Model");
	glUniformMatrix4fv(m_PVMMatrixLoc, 1, GL_FALSE, glm::value_ptr(m_objModelMat));

	// Pass in camera position via uniform for specular lighting
	glUniform3fv(glGetUniformLocation(_Program, "CameraPos"), 1, glm::value_ptr(_Camera->m_cameraPos));

	// Colour (if any)
	glUniform4fv(glGetUniformLocation(_Program, "Color"), 1, glm::value_ptr(glm::vec4(1, 0, 0, 1)));

	// Lighting manager manages the shader uniforms
	_lightManager->LightingShader(_Program);


	// Render the triangles
	glDrawArrays(m_drawType, 0, m_drawCount);	// GL_PATCHES
	glBindVertexArray(0);
	glUseProgram(0);
}