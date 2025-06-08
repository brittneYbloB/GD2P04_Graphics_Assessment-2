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
#include <fstream>
#include "CModelMesh.h"
#include "CTextureLoader.h"

class CHeightMap
{
private:

	struct HeightMapInfo
	{
		std::string FilePath;
		unsigned int Width		= 512;
		unsigned int Depth		= 512;
		float HeightScale		= 0.0025f;
		float HeightOffset		= 0.0f;
		float CellSpacing		= 0.0025f;
	};

	std::vector <float> HeightMap;
	std::vector<float> SmoothedMap;
	bool m_smoothenMore = false;
	HeightMapInfo m_info;
	std::vector<GLuint> m_Indices;

	GLuint m_VAO = 0;
	GLuint m_VBO = 0;
	GLuint m_EBO = 0;

	std::vector<VertexFormat> m_Vertices;
	
	GLuint m_texture;
	GLuint m_Texture_Dirt;
	GLuint m_Texture_Grass;
	GLuint m_Texture_Rocks;
	GLuint m_Texture_Snow;


	// Transformation Properties
	glm::vec3 m_objPosition = { 0.0f, -0.5f, 1.0f };
	glm::mat4 m_ScaleMat2D;
	glm::mat4 m_ScaleMat3D;
	glm::mat4 m_TranslationMat;
	float m_fObjRotationAngle = 0.0f;
	glm::mat4 m_RotationMat;
	glm::mat4 m_objModelMat;
	glm::vec3 m_objScale3D = glm::vec3(1.0f, 1.0f, 1.0f);

	// PVM
	glm::mat4 m_PVMMatrix;
	GLint m_PVMMatrixLoc;

public:
	CHeightMap(std::string _filePath);
	~CHeightMap() {};

	bool LoadHeightMap();
	void BuildVertexData();
	void BuildNormalsAtRuntime(std::vector<VertexFormat> Vertices);
	void BuildElementIndexData();
	float Average(unsigned int _Row, unsigned int _Col);

	void UpdateObject();
	void Render(GLuint _Program, CCamera* _Camera, CLightManager* _lightManager);
};

