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

#include "CTrianglePatchTessellationMesh.h"
#include "CTextureLoader.h"

/***************************************************************************
* CTrianglePatchTessellationMesh: Constructor which generates vertices for triangle patches
* @parameters:
* @return:
****************************************************************************/
CTrianglePatchTessellationMesh::CTrianglePatchTessellationMesh()
{
	m_texture = CTextureLoader::LoadTexture("Images/Courage.jpg", m_texture);

	Vertices.push_back(VertexPoint(glm::vec3(glm::sin(glm::radians(0.0f)), glm::cos(glm::radians(0.0f)), 0.0f), glm::vec2(0.5f, 1.0f)));
	Vertices.push_back(VertexPoint(glm::vec3(glm::sin(glm::radians(240.0f)), glm::cos(glm::radians(240.0f)), 0.0f), glm::vec2(0.0f, 0.0f)));
	Vertices.push_back(VertexPoint(glm::vec3(glm::sin(glm::radians(120.0f)), glm::cos(glm::radians(120.0f)), 0.0f), glm::vec2(1.0f, 0.0f)));


	glPatchParameteri(GL_PATCH_VERTICES, 3);	// Sets Triangle patch

	m_drawCount = (unsigned int)Vertices.size();
	m_drawType = GL_PATCHES;

	// Generate VAO, VBO and vertex-attribute pointers
	GenerateMesh();

}


/***************************************************************************
* ~CTrianglePatchTessellationMesh: 
* @parameters:
* @return:
****************************************************************************/
CTrianglePatchTessellationMesh::~CTrianglePatchTessellationMesh()
{
}


/***************************************************************************
* GenerateMesh: VAO, VBO, and vertex attribute pointers
* @parameters:
* @return:
****************************************************************************/
void CTrianglePatchTessellationMesh::GenerateMesh()
{
	/* VAO */
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	///* VBO */
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_drawCount * 3 * sizeof(VertexPoint), &Vertices[0], GL_STATIC_DRAW);

	// Set Vertex Attribute Data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPoint), (void*)0);
	glEnableVertexAttribArray(0);
	// ** Second vertex attribute pointer for texture, with updated strides (3 -> 5)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPoint), (void*)offsetof(VertexPoint, VertexPoint::TextureCoord));
	glEnableVertexAttribArray(1);					


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


/***************************************************************************
* UpdateObject: Update object
* @parameters: float _rotationAngle, glm::vec3 _axisRotation, float _deltaTime
* @return:
****************************************************************************/
void CTrianglePatchTessellationMesh::UpdateObject(float _rotationAngle, glm::vec3 _axisRotation, float _deltaTime)
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
void CTrianglePatchTessellationMesh::Render(GLuint _Program, CCamera* _Camera, CLightManager* _lightManager)
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

	// Colour (if any)
	glUniform4fv(glGetUniformLocation(_Program, "Color"), 1, glm::value_ptr(glm::vec4(1, 0, 0, 1)));

	// Lighting manager manages the shader uniforms
	_lightManager->LightingShader(_Program);


	// Render the triangles
	glDrawArrays(m_drawType, 0, m_drawCount);	// GL_PATCHES
	glBindVertexArray(0);
	glUseProgram(0);
}
