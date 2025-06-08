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

#include "CModelMesh.h"

/***************************************************************************
* SetVertexAttributes: Generate and set vertex attributes
* @parameters:
* @return:
****************************************************************************/
void CModelMesh::SetVertexAttributes()
{
	//// Vertex Attributes
	//glVertexAttribPointer(
	//	2, 
	//	3, 
	//	GL_FLOAT, 
	//	GL_FALSE, 
	//	sizeof(VertexFormat), 
	//	(void*)(offsetof(VertexFormat, VertexFormat::Normal)));
	//glEnableVertexAttribArray(2);

	// Vertex Attributes (** Cube vertex attributes by default)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::Position)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::TexCoord)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::Normal)));
	glEnableVertexAttribArray(1);
}


/***************************************************************************
* GenerateMesh: Generate and bind VAO, EBO, and VBO to set attributes
* @parameters:
* @return:
****************************************************************************/
void CModelMesh::GenerateMesh()
{
	/* VAO */
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	/* GENERATE EBO AND ASSIGN DATA */
	//glGenBuffers(1, &m_EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_Cube), &Indices_Cube, GL_STATIC_DRAW);

	/* VBO */
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * m_Vertices.size(), &m_Vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * m_Vertices.size(), &m_Vertices[0], GL_STATIC_DRAW);

	// Set attribute pointers
	SetVertexAttributes();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


/***************************************************************************
* Render: Use the program, bind the VAO and draw the elements
* @parameters: Program to use, camera, model matrix, PVM matrix, matrix location,
* and texture of object
* @return:
****************************************************************************/
void CModelMesh::Render(GLuint _Program, CCamera* _Camera, CLightManager* _lightManager)
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

	// Texture
	if (!m_is3DTextured)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glUniform1i(glGetUniformLocation(_Program, "ImageTexture"), 0);
	}
	// Pass in camera position via uniform for specular lighting
	glUniform3fv(glGetUniformLocation(_Program, "CameraPos"), 1, glm::value_ptr(_Camera->m_cameraPos));

	//
	glUniform1f(glGetUniformLocation(_Program, "time"), glfwGetTime());

	// Lighting manager manages the shader uniforms
	_lightManager->LightingShader(_Program);

	// Render the object
	glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size()); // mode = kind of primitives to render, first = starting index in enabled arrays, count = # of indices to be rendered

	glBindVertexArray(0);
}

/***************************************************************************
* UpdateObject: Update the transformation matrices of the object (PVM)
* @parameters: Rotation angle, desired axis of rotation, and delta time
* @return:
****************************************************************************/
void CModelMesh::UpdateObject(float _rotationAngle, glm::vec3 _axisRotation, float _deltaTime)
{
	// Translation matrix
	m_TranslationMat = glm::translate(glm::mat4(), m_objPosition);

	// Rotation matrix
	//m_RotationMat = glm::rotate(glm::mat4(), (float)glfwGetTime() * glm::radians(_rotationAngle), _axisRotation);
	m_RotationMat = glm::rotate(glm::mat4(), (float)glm::radians(_rotationAngle), _axisRotation);

	// Calculating the Scaling Matrix
	m_ScaleMat2D = glm::scale(glm::mat4(), m_objScale2D);
	m_ScaleMat3D = glm::scale(glm::mat4(), m_objScale3D);

	/* Calculate Model Matrix */
	m_objModelMat = m_TranslationMat * m_RotationMat * m_ScaleMat3D;
}