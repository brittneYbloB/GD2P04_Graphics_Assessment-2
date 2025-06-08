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

#include "CHeightmapTessellation.h"
#include <stb_image.h>

/***************************************************************************
* CHeightMap:
* @parameters: Filepath
* @return:
****************************************************************************/
CHeightmapTessellation::CHeightmapTessellation(std::string _filePath)
{
	// load height map texture
	std::string str = "Images/IcelandHeightmap.png";
	const char* c = str.c_str();
	unsigned char* data = stbi_load(c, &width, &height, &nChannels, 0);

	// vertex generation
	std::vector<float> vertices;
	float yScale = 64.0f / 256.0f, yShift = 16.0f;  // apply a scale+shift to the height data
	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			// retrieve texel for (i,j) tex coord
			unsigned char* texel = data + (j + width * i) * nChannels;
			// raw height at coordinate
			unsigned char y = texel[0];

			// vertex
			vertices.push_back(-height / 2.0f + i);        // v.x
			vertices.push_back((int)y * yScale - yShift); // v.y
			vertices.push_back(-width / 2.0f + j );        // v.z
		}
	}

	stbi_image_free(data);

	// index generation
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < height - 1; i++)       // for each row a.k.a. each strip
	{
		for (unsigned int j = 0; j < width; j++)      // for each column
		{
			for (unsigned int k = 0; k < 2; k++)      // for each side of the strip
			{
				indices.push_back(j + width * (i + k));
			}
		}
	}

	NUM_STRIPS = height - 1;
	NUM_VERTS_PER_STRIP = width * 2;

	// register VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER,
		vertices.size() * sizeof(float),       // size of vertices buffer
		&vertices[0],                          // pointer to first element
		GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(unsigned int), // size of indices buffer
		&indices[0],                           // pointer to first element
		GL_STATIC_DRAW);
}




/***************************************************************************
* UpdateObject: Update the transformation matrices of the object (PVM)
* @parameters: Rotation angle, desired axis of rotation, and delta time
* @return:
****************************************************************************/
void CHeightmapTessellation::UpdateObject()
{
	// Translation matrix
	m_TranslationMat = glm::translate(glm::mat4(), m_objPosition);

	// Rotation matrix
	//m_RotationMat = glm::rotate(glm::mat4(), (float)glfwGetTime() * glm::radians(_rotationAngle), _axisRotation);
	//m_RotationMat = glm::rotate(glm::mat4(), (float)glm::radians(_rotationAngle), _axisRotation);

	// Calculating the Scaling Matrix
	m_ScaleMat3D = glm::scale(glm::mat4(), m_objScale3D);

	/* Calculate Model Matrix */
	m_objModelMat = m_TranslationMat * m_RotationMat * m_ScaleMat3D;
}


/***************************************************************************
* Render: Draw the heightmap
* @parameters: Program, camera, light manager
* @return:
****************************************************************************/
void CHeightmapTessellation::Render(GLuint _Program, CCamera* _Camera, CLightManager* _lightManager)
{
	glUseProgram(_Program);
	glBindVertexArray(m_VAO);

	/** PVM **/
	m_PVMMatrix = _Camera->m_projectionMat * _Camera->m_viewMat * m_objModelMat;
	m_PVMMatrixLoc = glGetUniformLocation(_Program, "PVMMatrix");
	glUniformMatrix4fv(m_PVMMatrixLoc, 1, GL_FALSE, glm::value_ptr(m_PVMMatrix));
	m_PVMMatrixLoc = glGetUniformLocation(_Program, "Model");
	glUniformMatrix4fv(m_PVMMatrixLoc, 1, GL_FALSE, glm::value_ptr(m_objModelMat));

	// Textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture_Snow);
	glUniform1i(glGetUniformLocation(_Program, "imageSnow"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_Texture_Rocks);
	glUniform1i(glGetUniformLocation(_Program, "imageRocks"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_Texture_Grass);
	glUniform1i(glGetUniformLocation(_Program, "imageGrass"), 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_Texture_Dirt);
	glUniform1i(glGetUniformLocation(_Program, "imageDirt"), 3);


	// Pass in camera position via uniform for specular lighting
	glUniform3fv(glGetUniformLocation(_Program, "CameraPos"), 1, glm::value_ptr(_Camera->m_cameraPos));

	// Lighting manager manages the shader uniforms
	_lightManager->LightingShader(_Program);


	// render the mesh triangle strip by triangle strip - each row at a time
	for (unsigned int strip = 0; strip < NUM_STRIPS; ++strip)
	{
		glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
			NUM_VERTS_PER_STRIP, // number of indices to render
			GL_UNSIGNED_INT,     // index data type
			(void*)(sizeof(unsigned int)
				* NUM_VERTS_PER_STRIP
				* strip)); // offset to starting index
	}

	// Unbind assets to prevent accidental use/modification
	glBindVertexArray(0);
	glUseProgram(0);
}
