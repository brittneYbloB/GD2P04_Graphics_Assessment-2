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

#include "CSkyBox.h"

// Cube vertices & index/element array of position only data
GLfloat Vertices_SkyBox[] = {
	// Index	 Position					Position Index
	/*-----------------------------------------------------*/

				// Front Quad
	/* 00 */	 -0.5f,  0.5f,  0.5f,		/* 00 */
	/* 01 */	 -0.5f, -0.5f,  0.5f,		/* 01 */
	/* 02 */	  0.5f, -0.5f,  0.5f,		/* 02 */
	/* 03 */	  0.5f,  0.5f,  0.5f,		/* 03 */

				// Back Quad
	/* 04 */	  0.5f,  0.5f, -0.5f,		/* 04 */
	/* 05 */	  0.5f, -0.5f, -0.5f,		/* 05 */
	/* 06 */	 -0.5f, -0.5f, -0.5f,		/* 06 */
	/* 07 */	 -0.5f,  0.5f, -0.5f,		/* 07 */

				// Right
	/* 08 */	  0.5f,  0.5f,  0.5f,		/* 03 */
	/* 09 */	  0.5f, -0.5f,  0.5f,		/* 02 */
	/* 10 */	  0.5f, -0.5f, -0.5f,		/* 05 */
	/* 11 */	  0.5f,  0.5f, -0.5f,		/* 04 */

				// Left
	/* 12 */	 -0.5f,  0.5f, -0.5f,		/* 07 */
	/* 13 */	 -0.5f, -0.5f, -0.5f,		/* 06 */
	/* 14 */	 -0.5f, -0.5f,  0.5f,		/* 01 */
	/* 15 */	 -0.5f,  0.5f,  0.5f,		/* 00 */

				// Top
	/* 16 */	 -0.5f, 0.5f, -0.5f,		/* 07 */
	/* 17 */	 -0.5f, 0.5f,  0.5f,		/* 00 */
	/* 18 */	  0.5f, 0.5f,  0.5f,		/* 03 */
	/* 19 */	  0.5f, 0.5f, -0.5f,		/* 04 */
	
				// Bottom
	/* 20 */	 -0.5f, -0.5f,  0.5f,		/* 01 */
	/* 21 */	 -0.5f, -0.5f, -0.5f,		/* 06 */
	/* 22 */	  0.5f, -0.5f, -0.5f,		/* 05 */
	/* 23 */	  0.5f, -0.5f,  0.5f,		/* 02 */
};
// Winding order facing inwards (as the viewing faces of skybox is inside)
GLuint Indices_SkyBox[] = {
	2, 1, 0,	// Front  Tri 1		// 0
	3, 2, 0,	// Front  Tri 2	 	// 1
	6, 5, 4,	// Back   Tri 1	 	// 2
	7, 6, 4,	// Back   Tri 2	 	// 3
	10, 9, 8,	// Right  Tri 1	 	// 4
	11, 10, 8,	// Right  Tri 2	 	// 5
	14, 13, 12,	// Left   Tri 1	 	// 6
	15, 14, 12,	// Left   Tri 2	 	// 7
	18, 17, 16,	// Top    Tri 1	 	// 8
	19, 18, 16,	// Top    Tri 2	 	// 9
	22, 21, 20,	// Bottom Tri 1	 	// 10
	23, 22, 20,	// Bottom Tri 2	 	// 11
};


/***************************************************************************
* CSkyBox: Create program, VBO, VAO, EBO, set file paths and load textures from files
* @parameters:
* @return:
****************************************************************************/
CSkyBox::CSkyBox(CCamera* _Camera)
{
	m_pCamera = _Camera;



	// Create and assing the VAO, VBO and EBO data
	/* VAO */
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	/* GENERATE EBO AND ASSIGN DATA */
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_SkyBox), &Indices_SkyBox, GL_STATIC_DRAW);
	/* VBO */
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_SkyBox), &Vertices_SkyBox, GL_STATIC_DRAW);

		// VertexAttribPointers: only need 1. (Position data)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Generate a single TextureID
		// Uses GL_TEXTURE_CUBE_MAP for all 6 faces
	SetFilePaths();
	LoadTextures();
}


/***************************************************************************
* SetFilePaths: Set the file paths for the textures (of the box faces)
* @parameters:
* @return:
****************************************************************************/
void CSkyBox::SetFilePaths()
{
	// Store cubemap file path array
	m_cubemapFilePathArray[0] = "MountainOutpost/Right.jpg";	/* Right */
	m_cubemapFilePathArray[1] = "MountainOutpost/Left.jpg";	/* Left */
	m_cubemapFilePathArray[2] = "MountainOutpost/Up.jpg";		/* Top */
	m_cubemapFilePathArray[3] = "MountainOutpost/Down.jpg";	/* Bottom */
	m_cubemapFilePathArray[4] = "MountainOutpost/Back.jpg";	/* Back */
	m_cubemapFilePathArray[5] = "MountainOutpost/Front.jpg";	/* Front */
}


/***************************************************************************
* LoadTextures: Load images and populate texture with the image data
* @parameters:
* @return:
****************************************************************************/
void CSkyBox::LoadTextures()
{
	// Create and bind a new texture template
	glGenTextures(1, &m_skyBoxTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyBoxTexture);

	// Load the Image Data
	int ImageWidth;
	int ImageHeight;
	int ImageComponents;
	stbi_set_flip_vertically_on_load(false);

	for (int i = 0; i < 6; i++)
	{
		std::string FullFilePath = "Images/Cubemaps/" + m_cubemapFilePathArray[i];
		unsigned char* ImageData = stbi_load(FullFilePath.c_str(),
			&ImageWidth, &ImageHeight, &ImageComponents, 0);

		// Check how many components the loaded image has (RGB or RGBA?)
		GLint LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

		// Populate the texture with the image data
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,	// basic arithmetic can be performed on enums/defines due to their values being int in the bckgrnd
			0, LoadedComponents, ImageWidth, ImageHeight, 0,
			LoadedComponents, GL_UNSIGNED_BYTE, ImageData);

		stbi_image_free(ImageData);
	}

	/* Texture Parameters */
	// Setting the address mode for this texture
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Generate the mipmaps, free the memory and unbind the texture
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}


/***************************************************************************
* Update: Update the model and PVM matrices
* @parameters: Delta time
* @return:
****************************************************************************/
void CSkyBox::Update(float _deltaTIme)
{
	m_objModelMat = glm::scale(glm::mat4(), glm::vec3(2000.0f, 2000.0f, 2000.0f));
	m_PVMMatrix = m_pCamera->m_projectionMat * m_pCamera->m_viewMat * m_objModelMat;
}


/***************************************************************************
* Render: Draw the skybox to the screen (binding VAO and texture map)
* @parameters:
* @return:
****************************************************************************/
void CSkyBox::Render(GLuint _program, CCamera* _Camera)
{
	glUseProgram(_program);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyBoxTexture);
	glUniform1i(glGetUniformLocation(_program, "Texture0"), 0);
	glUniformMatrix4fv(glGetUniformLocation(_program, "PVM"), 1, GL_FALSE, glm::value_ptr(m_PVMMatrix));

	glUniform3fv(glGetUniformLocation(_program, "CameraPos"), 1, glm::value_ptr(_Camera->m_cameraPos));

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glUseProgram(0);
}