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

#include "CQuad.h"
#include "CShaderLoader.h"
#include "CTextureLoader.h"


/***************************************************************************
* CQuad: Create quad
* @parameter:
* @return:
****************************************************************************/
CQuad::CQuad()
{
	// VAO, EBO and VBO
	/* VAO */
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	/* GENERATE EBO AND ASSIGN DATA */
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_Quad), Indices_Quad, GL_STATIC_DRAW);

	/* VBO */
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Quad), Vertices_Quad, GL_STATIC_DRAW);

	// Vertex Attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);		// Offset from the beginning of each vertex
	glEnableVertexAttribArray(0);															// Stride of a single vertex (Sum of all vertex componenets in bytes)
	// ** Second vertex attribute pointer for color, with updated strides (3 -> 6)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// ** Third vertex attribute pointer for the texture coordinate
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Set texture
	m_texture = CTextureLoader::LoadTexture("Images/static.png", m_texture);
}

/***************************************************************************
* Render: Render quad
* @parameter:
* @return:
****************************************************************************/
void CQuad::Render(GLuint _postProcessingProgram, GLuint _framebufferColorAttachmentTexture, float _deltaTime)
{
	m_programQuad = _postProcessingProgram;

	// Program
	glUseProgram(m_programQuad);
	glBindVertexArray(m_VAO);

	// Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _framebufferColorAttachmentTexture);	// changed for framebuffer purposes
	glUniform1i(glGetUniformLocation(m_programQuad, "ImageTexture"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_texture);	// changed for framebuffer purposes
	glUniform1i(glGetUniformLocation(m_programQuad, "StaticImage"), 1);

	//
	glUniform1f(glGetUniformLocation(m_programQuad, "time"), glfwGetTime());

	// Render the triangles
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Unbind assets to prevent accidental use/modification
	glBindVertexArray(0);
	glUseProgram(0);
}
