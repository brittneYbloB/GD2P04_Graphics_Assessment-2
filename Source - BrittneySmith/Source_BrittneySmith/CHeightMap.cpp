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

#include "CHeightMap.h"


/***************************************************************************
* CHeightMap:
* @parameters: Filepath
* @return:
****************************************************************************/
CHeightMap::CHeightMap(std::string _filePath)
{
	m_info.FilePath = _filePath;
	m_Texture_Dirt = CTextureLoader::LoadTexture("Models/Terrain/Dirt_Dark.png", m_Texture_Dirt);
	m_Texture_Grass = CTextureLoader::LoadTexture("Models/Terrain/Grass_Dark.png", m_Texture_Grass);
	m_Texture_Rocks = CTextureLoader::LoadTexture("Models/Terrain/Stone.png", m_Texture_Rocks);
	m_Texture_Snow = CTextureLoader::LoadTexture("Models/Terrain/Snow.png", m_Texture_Snow);

	if (LoadHeightMap())
	{
		BuildVertexData();
		BuildElementIndexData();
	}
}


/***************************************************************************
* LoadHeightMap:
* @parameters:
* @return: Boolean
****************************************************************************/
bool CHeightMap::LoadHeightMap()
{
	unsigned int VertexCount = m_info.Width * m_info.Depth; // match the width and depth of the mesh
	// Data Structure to contain the height info for each vertex
	std::vector <unsigned char> HeightValue(VertexCount);

	// Open the data file and read
	std::ifstream File;
	File.open(m_info.FilePath, std::ios_base::binary);	// filepath is the path the the file to load (inc extension)
	if (File)
	{
		// Read in the raw bytes
		File.read((char*)&HeightValue[0], (std::streamsize)HeightValue.size());
		File.close();	// File no longer needed
	}
	else
	{
		std::cout << "Error: Height map failed to load: " << m_info.FilePath << std::endl;
		return false;
	}

	// Transfer data into a float array. Apply Scaling and Offset
	HeightMap.resize(VertexCount, 0);
	for (unsigned int i = 0; i < VertexCount; i++)
	{
		HeightMap[i] = (float)HeightValue[i] * m_info.HeightScale + m_info.HeightOffset;	// heightscale is scalar to apply to height values, and offset is Y translation away from origin
	}

	// SMOOTHING GOES HERE
	SmoothedMap.resize(HeightMap.size());
	for (unsigned int Row = 0; Row < m_info.Width; Row++)
	{
		for (unsigned int Col = 0; Col < m_info.Depth; Col++)
		{
			SmoothedMap[Row * m_info.Depth + Col] = Average(Row, Col);
		}
	}
	// Replace the previous Heightmap with the smoothed version
	HeightMap = SmoothedMap;


	return true;
}


/***************************************************************************
* BuildVertexData:
* @parameters:
* @return:
****************************************************************************/
void CHeightMap::BuildVertexData()
{
	unsigned int VertexCount = m_info.Width * m_info.Depth;
	std::vector<VertexFormat> Vertices(VertexCount); // vertex array for VBO data

	// Shift (0,0,0) to center point
	float HalfWidth = (m_info.Width - 1) * m_info.CellSpacing * 0.5f;
	float HalfDepth = (m_info.Depth - 1) * m_info.CellSpacing * 0.5f;

	// Map 0-1 texture coords to mesh width/depth
	float TexU = 1.0f / (float)(m_info.Width - 1);
	float TexV = 1.0f / (float)(m_info.Depth - 1);

	for (unsigned int Row = 0; Row < m_info.Width; Row++)
	{
		float PosZ = HalfDepth - (Row * m_info.CellSpacing);
		
		for (unsigned int Col = 0; Col < m_info.Depth; Col++)
		{
			int Index = Row * m_info.Width + Col;

			float PosX = -HalfWidth + (Col * m_info.CellSpacing);
			float PosY = HeightMap[Index];

			Vertices[Index].Position = glm::vec3(PosX, PosY, PosZ);
			Vertices[Index].TexCoord = glm::vec2(Col * TexU, Row * TexV);	// Build the single Vertex
			Vertices[Index].Normal = glm::vec3(0.0f, 1.0f, 0.0f);	// Normal Data comes later
		}
	}

	// Next function
	BuildNormalsAtRuntime(Vertices);
}


/***************************************************************************
* BuildNormalsAtRuntime: 
* @parameters: Vertices
* @return: 
****************************************************************************/
void CHeightMap::BuildNormalsAtRuntime(std::vector<VertexFormat> Vertices)
{
	// Calculate normals using central difference
	float InvCellSpacing = 1.0f / (2.0f * m_info.CellSpacing);
	for (unsigned int Row = 2; Row < m_info.Width - 1; Row++)
	{
		for (unsigned int Col = 2; Col < m_info.Depth - 1; Col++)
		{
			float RowNeg = HeightMap[(Row - 1) * m_info.Depth + Col];
			float RowPos = HeightMap[(Row + 1) * m_info.Depth + Col];
			float ColNeg = HeightMap[Row * m_info.Depth + Col - 1];
			float ColPos = HeightMap[Row * m_info.Depth + Col + 1];

			glm::vec3 TanZ(0.0f, (RowNeg - RowPos) * InvCellSpacing, 1.0f);
			glm::vec3 TanX(1.0f, (ColPos - ColNeg) * InvCellSpacing, 0.0f);

			glm::vec3 Normal = glm::cross(TanZ, TanX);
			Normal = glm::normalize(Normal);

			Vertices[Row * m_info.Depth + Col].Normal = Normal;
		}
	}

	

	m_Vertices = Vertices;
}


/***************************************************************************
* Average: Average point based on surrounding points
* @parameters:
* @return: Aveaged point
****************************************************************************/
float CHeightMap::Average(unsigned int _Row, unsigned int _Col)
{
	float value = 0.0f;
	float cellAverage = 1.0f;

	unsigned int bounds = m_info.Width * m_info.Depth;

	// Sample top row
	if (((_Row - 1) + (_Col - 1) * m_info.Width) >= 0 &&
		((_Row - 1) + (_Col - 1) * m_info.Width) < bounds)
	{
		if (!m_smoothenMore) value += HeightMap[(_Row - 1) + (_Col - 1) * m_info.Width];
		else value += SmoothedMap[(_Row - 1) + (_Col - 1) * m_info.Width];
		++cellAverage;
	}

	if (((_Row - 0) + (_Col - 1) * m_info.Width) >= 0 &&
		((_Row - 0) + (_Col - 1) * m_info.Width) < bounds)
	{
		if (!m_smoothenMore) value += HeightMap[(_Row)+(_Col - 1) * m_info.Width];
		else value += SmoothedMap[(_Row)+(_Col - 1) * m_info.Width];
		++cellAverage;
	}

	if (((_Row + 1) + (_Col - 1) * m_info.Width) >= 0 &&
		((_Row + 1) + (_Col - 1) * m_info.Width) < bounds)
	{
		if (!m_smoothenMore) value += HeightMap[(_Row + 1) + (_Col - 1) * m_info.Width];
		else value += SmoothedMap[(_Row + 1) + (_Col - 1) * m_info.Width];
		++cellAverage;
	}

	// Sample middle row
	if (((_Row - 1) + (_Col - 0) * m_info.Width) >= 0 &&
		((_Row - 1) + (_Col - 0) * m_info.Width) < bounds)
	{
		if (!m_smoothenMore) value += HeightMap[(_Row - 1) + (_Col)*m_info.Width];
		else value += SmoothedMap[(_Row - 1) + (_Col)*m_info.Width];
		++cellAverage;
	}

	// Sample center point (will always be in bounds)
	value += HeightMap[_Row + _Col * m_info.Width];

	if (((_Row + 1) + (_Col - 0) * m_info.Width) >= 0 &&
		((_Row + 1) + (_Col - 0) * m_info.Width) < bounds)
	{
		if (!m_smoothenMore) value += HeightMap[(_Row + 1) + (_Col)*m_info.Width];
		else value += SmoothedMap[(_Row + 1) + (_Col)*m_info.Width];
		++cellAverage;
	}

	// Sample bottom row
	if (((_Row - 1) + (_Col + 1) * m_info.Width) >= 0 &&
		((_Row - 1) + (_Col + 1) * m_info.Width) < bounds)
	{
		if (!m_smoothenMore) value += HeightMap[(_Row - 1) + (_Col + 1) * m_info.Width];
		else value += SmoothedMap[(_Row - 1) + (_Col + 1) * m_info.Width];
		++cellAverage;
	}

	if (((_Row - 0) + (_Col + 1) * m_info.Width) >= 0 &&
		((_Row - 0) + (_Col + 1) * m_info.Width) < bounds)
	{
		if (!m_smoothenMore) value += HeightMap[(_Row)+(_Col + 1) * m_info.Width];
		else value += SmoothedMap[(_Row)+(_Col + 1) * m_info.Width];
		++cellAverage;
	}

	if (((_Row + 1) + (_Col + 1) * m_info.Width) >= 0 &&
		((_Row + 1) + (_Col + 1) * m_info.Width) < bounds)
	{
		if (!m_smoothenMore) value += HeightMap[(_Row + 1) + (_Col + 1) * m_info.Width];
		else value += SmoothedMap[(_Row + 1) + (_Col + 1) * m_info.Width];
		++cellAverage;
	}

	float average = value / cellAverage;

	return average;
}


/***************************************************************************
* BuildElementIndexData: 
* @parameters:
* @return:
****************************************************************************/
void CHeightMap::BuildElementIndexData()
{
	GLenum DrawType = GL_TRIANGLES;
	unsigned int FaceCount = (m_info.Width - 1) * (m_info.Depth - 1) * 2;
	int IndexCount = FaceCount * 3;
	std::vector<GLuint> Indices(IndexCount);

	int Index = 0;
	for (unsigned int Row = 0; Row < (m_info.Width - 1); Row++)
	{
		for (unsigned int Col = 0; Col < (m_info.Depth - 1); Col++)
		{
			Indices[Index++] = Row * m_info.Depth + Col;
			Indices[Index++] = Row * m_info.Depth + Col + 1;
			Indices[Index++] = (Row + 1) * m_info.Depth + Col;

			Indices[Index++] = (Row + 1) * m_info.Depth + Col;
			Indices[Index++] = Row * m_info.Depth + Col + 1;
			Indices[Index++] = (Row + 1) * m_info.Depth + Col + 1;
		}
	}
	m_Indices = Indices;

	/* VAO */
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	/* Build the EBO */
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(GLuint), &Indices[0], GL_STATIC_DRAW);
	std::cout << "sizeof(Indices) (local var) being used in EBO buffer data: " << Indices.size() << std::endl;

	/* Build the VBO */
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);	// error in renderdoc if changed to GL_ELEMENT_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * m_Vertices.size(), &m_Vertices[0], GL_STATIC_DRAW);

	/* Vertex Attributes */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::Position)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::TexCoord)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::Normal)));
	glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


/***************************************************************************
* UpdateObject: Update the transformation matrices of the object (PVM)
* @parameters: Rotation angle, desired axis of rotation, and delta time
* @return:
****************************************************************************/
void CHeightMap::UpdateObject()
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
void CHeightMap::Render(GLuint _Program, CCamera* _Camera, CLightManager* _lightManager)
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

	// Render the object
	//glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size()); // mode = kind of primitives to render, first = starting index in enabled arrays, count = # of indices to be rendered
	glDrawElements(GL_TRIANGLES, m_Indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, 0);
	//std::cout << "m_Indices.size() being used in DrawElements(): " << m_Indices.size() << std::endl;

	// Unbind assets to prevent accidental use/modification
	glBindVertexArray(0);
	glUseProgram(0);
}
