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

#include "CMeshCube.h"
#include "CTextureLoader.h"

// Cube vertices & index/element array
GLfloat Vertices_Cube[] = {
	// Index	 Position					Texture Coordinates		Position Index
	/*-----------------------------------------------------------------------------*/

				// Front Quad
	/* 00 */	 -0.5f,  0.5f,  0.5f,		0.0f, 1.0f,				/* 00 */
	/* 01 */	 -0.5f, -0.5f,  0.5f,		0.0f, 0.0f,	 			/* 01 */
	/* 02 */	  0.5f, -0.5f,  0.5f,		1.0f, 0.0f,	 			/* 02 */
	/* 03 */	  0.5f,  0.5f,  0.5f,		1.0f, 1.0f,	 			/* 03 */

				// Back Quad
				/* 04 */	  0.5f,  0.5f, -0.5f,		0.0f, 1.0f,				/* 04 */
				/* 05 */	  0.5f, -0.5f, -0.5f,		0.0f, 0.0f,	 			/* 05 */
				/* 06 */	 -0.5f, -0.5f, -0.5f,		1.0f, 0.0f,	 			/* 06 */
				/* 07 */	 -0.5f,  0.5f, -0.5f,		1.0f, 1.0f,	 			/* 07 */

							// Right
							/* 08 */	  0.5f,  0.5f,  0.5f,		0.0f, 1.0f,				/* 03 */
							/* 09 */	  0.5f, -0.5f,  0.5f,		0.0f, 0.0f,	 			/* 02 */
							/* 10 */	  0.5f, -0.5f, -0.5f,		1.0f, 0.0f,	 			/* 05 */
							/* 11 */	  0.5f,  0.5f, -0.5f,		1.0f, 1.0f,	 			/* 04 */

										// Left
										/* 12 */	 -0.5f,  0.5f, -0.5f,		0.0f, 1.0f,				/* 07 */
										/* 13 */	 -0.5f, -0.5f, -0.5f,		0.0f, 0.0f,	 			/* 06 */
										/* 14 */	 -0.5f, -0.5f,  0.5f,		1.0f, 0.0f,	 			/* 01 */
										/* 15 */	 -0.5f,  0.5f,  0.5f,		1.0f, 1.0f,	 			/* 00 */

													// Top
													/* 16 */	 -0.5f, 0.5f, -0.5f,		0.0f, 1.0f,				/* 07 */
													/* 17 */	 -0.5f, 0.5f,  0.5f,		0.0f, 0.0f,	 			/* 00 */
													/* 18 */	  0.5f, 0.5f,  0.5f,		1.0f, 0.0f,	 			/* 03 */
													/* 19 */	  0.5f, 0.5f, -0.5f,		1.0f, 1.0f,	 			/* 04 */

																// Bottom
																/* 20 */	 -0.5f, -0.5f,  0.5f,		0.0f, 1.0f,				/* 01 */
																/* 21 */	 -0.5f, -0.5f, -0.5f,		0.0f, 0.0f,	 			/* 06 */
																/* 22 */	  0.5f, -0.5f, -0.5f,		1.0f, 0.0f,	 			/* 05 */
																/* 23 */	  0.5f, -0.5f,  0.5f,		1.0f, 1.0f,	 			/* 02 */
};
GLuint Indices_Cube[] = {
	0, 1, 2,	// Front  Tri 1		// 0
	0, 2, 3,	// Front  Tri 2	 	// 1
	4, 5, 6,	// Back   Tri 1	 	// 2
	4, 6, 7,	// Back   Tri 2	 	// 3
	8, 9, 10,	// Right  Tri 1	 	// 4
	8, 10, 11,	// Right  Tri 2	 	// 5
	12, 13, 14,	// Left   Tri 1	 	// 6
	12, 14, 15,	// Left   Tri 2	 	// 7
	16, 17, 18,	// Top    Tri 1	 	// 8
	16, 18, 19,	// Top    Tri 2	 	// 9
	20, 21, 22,	// Bottom Tri 1	 	// 10
	20, 22, 23,	// Bottom Tri 2	 	// 11
};



CMeshCube::CMeshCube()
{
	// Set properties
	m_texture = CTextureLoader::LoadTexture("Images/PerlinNoiseTexture.jpg", m_texture);
	m_objScale3D = glm::vec3(0.5f, 0.5f, 0.5f);
	m_objPosition = glm::vec3(-0.25f, 0.25f, -1.0f);

	// Generate mesh (VAO, EBO, etc)
	GenerateMesh();
	// Set vertex attribute pointers
	SetVertexAttributes();
}



/***************************************************************************
* SetVertexAttributes: Generate and set vertex attributes
* @parameters:
* @return:
****************************************************************************/
void CMeshCube::SetVertexAttributes()
{
	// Vertex Attributes (** Cube vertex attributes by default)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}


/***************************************************************************
* GenerateMesh: Generate and bind VAO, EBO, and VBO to set attributes
* @parameters:
* @return:
****************************************************************************/
void CMeshCube::GenerateMesh()
{
	/* VAO */
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	/* GENERATE EBO AND ASSIGN DATA */
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_Cube), &Indices_Cube, GL_STATIC_DRAW);
	/* VBO */
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Cube), &Vertices_Cube, GL_STATIC_DRAW);

	// Set attribute pointers
	SetVertexAttributes();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


/***************************************************************************
* UpdateObject: Update the transformation matrices of the object (PVM)
* @parameters: Rotation angle, desired axis of rotation, and delta time
* @return:
****************************************************************************/
void CMeshCube::UpdateObject(float _rotationAngle, glm::vec3 _axisRotation, float _deltaTime)
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
* Render: Use the program, bind the VAO and draw the elements
* @parameters: Program to use, camera, model matrix, PVM matrix, matrix location,
* and texture of object
* @return:
****************************************************************************/
void CMeshCube::Render(GLuint _Program, CCamera* _Camera, CLightManager* _lightManager)
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

	// Pass in texture of fragment texture
	glActiveTexture(GL_TEXTURE0);
	// Act based on reflection validity
	if (m_bIsReflective) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}
	glUniform1i(glGetUniformLocation(_Program, "ImageTexture"), 0);

	// Pass in camera position via uniform for specular lighting
	glUniform3fv(glGetUniformLocation(_Program, "CameraPos"), 1, glm::value_ptr(_Camera->m_cameraPos));

	// Stencil colour (if any)
	glUniform4fv(glGetUniformLocation(_Program, "Color"), 1, glm::value_ptr(glm::vec4(1, 0, 0, 1)));

	// Lighting manager manages the shader uniforms
	_lightManager->LightingShader(_Program);

	// Render the triangles
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}