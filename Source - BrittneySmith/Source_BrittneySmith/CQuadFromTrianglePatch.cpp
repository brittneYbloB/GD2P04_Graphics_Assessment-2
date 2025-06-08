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

#include "CQuadFromTrianglePatch.h"
#include "CTextureLoader.h"


/***************************************************************************
* CQuadFromTrianglePatch: Constructor which generates vertices for triangle patches
* @parameters:
* @return:
****************************************************************************/
CQuadFromTrianglePatch::CQuadFromTrianglePatch()
{
	m_texture = CTextureLoader::LoadTexture("Images/Courage.jpg", m_texture);

	Vertices.push_back(VertexPoint(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec2()));
	Vertices.push_back(VertexPoint(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2()));
	Vertices.push_back(VertexPoint(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec2()));

	glPatchParameteri(GL_PATCH_VERTICES, 3);	// Sets Triangle patch

	//m_drawCount = (unsigned int)Vertices.size();
	m_drawCount = 6; // was 4
	m_drawType = GL_PATCHES;

	// Generate VAO, VBO and vertex-attribute pointers
	GenerateMesh();

}


/***************************************************************************
* ~CQuadFromTrianglePatch: Deconstructor
* @parameters:
* @return:
****************************************************************************/
CQuadFromTrianglePatch::~CQuadFromTrianglePatch()
{
}


/***************************************************************************
* GenerateMesh: VAO, VBO, vertex attribute pointers
* @parameters:
* @return:
****************************************************************************/
void CQuadFromTrianglePatch::GenerateMesh()
{
	/* VAO */
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	/* VBO */
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Quad), Vertices_Quad, GL_STATIC_DRAW);

	// Vertex Attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);		// Offset from the beginning of each vertex
	glEnableVertexAttribArray(0);															// Stride of a single vertex (Sum of all vertex componenets in bytes)
	// ** Second vertex attribute pointer for texture, with updated strides (3 -> 5)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


/***************************************************************************
* UpdateObject: Update
* @parameters: float _rotationAngle, glm::vec3 _axisRotation, float _deltaTime
* @return:
****************************************************************************/
void CQuadFromTrianglePatch::UpdateObject(float _rotationAngle, glm::vec3 _axisRotation, float _deltaTime)
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
void CQuadFromTrianglePatch::Render(GLuint _Program, CCamera* _Camera, CLightManager* _lightManager)
{
	glUseProgram(_Program);
	glBindVertexArray(m_VAO);

	// Pass in texture of fragment texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniform1i(glGetUniformLocation(_Program, "ImageTexture"), 0);

	/** PVM **/
	m_PVMMatrix = _Camera->m_projectionMat * _Camera->m_viewMat * m_objModelMat;
	m_PVMMatrixLoc = glGetUniformLocation(_Program, "PVMMatrix");
	glUniformMatrix4fv(m_PVMMatrixLoc, 1, GL_FALSE, glm::value_ptr(m_PVMMatrix));
	// Pass in model matrix for lighting
	m_PVMMatrixLoc = glGetUniformLocation(_Program, "Model");
	glUniformMatrix4fv(m_PVMMatrixLoc, 1, GL_FALSE, glm::value_ptr(m_objModelMat));

	// Pass in camera position via uniform for specular lighting
	glUniform3fv(glGetUniformLocation(_Program, "CameraPos"), 1, glm::value_ptr(_Camera->m_cameraPos));

	// Lighting manager manages the shader uniforms
	_lightManager->LightingShader(_Program);


	// Render the triangles
	glDrawArrays(m_drawType, 0, m_drawCount);	// GL_PATCHES
	glBindVertexArray(0);
	glUseProgram(0);
}