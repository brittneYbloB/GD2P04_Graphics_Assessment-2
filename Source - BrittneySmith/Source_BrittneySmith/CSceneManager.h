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
//#include <tiny_obj_loader.h>
// Math Libraries
#include <gtc/matrix_transform.hpp>
#include <queue>

// Classes
#include "CTextureLoader.h"
#include "CShaderLoader.h"
#include "CTimeManager.h"
#include "CCamera.h"
#include "CSphere.h"
#include "CLightManager.h"
#include "CSkyBox.h"
#include "CInputManager.h"
#include "CModelLoading.h"
#include "CHeightMap.h"
#include "CPerlinNoise.h"
#include "CMeshCube.h"
#include "CQuad.h"
#include "CPointsMesh.h"
#include "CQuadPatchTessellationMesh.h"
#include "CTrianglePatchTessellationMesh.h"
#include "CHeightmapTessellation.h"
#include "CFramebuffer.h"
#include "CQuadFromTrianglePatch.h"

enum class SCENE
{
	e_SCENE_1 = 1,
	e_SCENE_2 = 2,
	e_SCENE_3 = 3,
};

enum class POSTPROCESSING
{
	e_NONE = 0,
	e_COLORINVERSION = 1,
	e_GREYSCALE = 2,
	e_RAIN = 3,
	e_TVCRT = 4,
};

class CSceneManager
{
private:
	
	// Objects
	CSphere* m_pSphere = nullptr;					// Sphere object
	CSphere* m_pSphereNormals = nullptr;					// Sphere object
	CSphere* m_pSphereExplosion = nullptr;					// Sphere object
	CMeshCube* m_pCubeWithPerlinTex = nullptr;		// Cube object
	CQuad* m_pQuad = nullptr;

	// Class pointers
	CInputManager* m_pInputManager = nullptr;		// Input
	CTimeManager* m_pTimeManager = nullptr;			// Time Variables for Animation
	CCamera* m_pCamera = nullptr;					// Camera
	CLightManager* m_pLightManager = nullptr;		// Lighting
	CSkyBox* m_pSkyBox = nullptr;					// Cubemap & filepath array
	CHeightMap* m_heightMap = nullptr;				// Heightmap
	CHeightmapTessellation* m_tessHeightMap = nullptr;				// Heightmap
	CHeightMap* m_perlinNoiseTerrain = nullptr;		
	CPerlinNoise* m_perlinNoise = nullptr;			// Heightmap

	// Mesh loader
	CModel m_skullModel;
	CModel m_doggieModel;

	//
	CPointsMesh* m_pointsGeometryShader = nullptr;
	CQuadPatchTessellationMesh* m_quadPatchTessellationMesh = nullptr;
	CTrianglePatchTessellationMesh* m_trianglePatchTessellationMesh = nullptr;
	CQuadFromTrianglePatch* m_quadTrianglePatcheTessellationMesh = nullptr;

	// Framebuffer Object
	CFramebuffer* m_framebufferObject = nullptr;

	// Textures
	GLuint m_textureCourage;

	// Programs
	GLuint m_programCombinedLights;
	GLuint m_programLitHeightmap;
	GLuint m_program3DTexture;
	GLuint m_programSkyBox;
	GLuint m_programSkyBoxFog;
	// Geometry Shader Programs
	GLuint m_programGeometryShader;
	GLuint m_programGeometryShaderShowingNormals;
	GLuint m_programGeometryShaderExplosion;
	// Tessellation Programs
	GLuint m_programQuadPatch_TS;
	GLuint m_programTrianglePatch_TS;
	GLuint m_programTerrain_TS;
	GLuint m_programQuadTrianglePatch_TS;
	// Post processing Programs
	GLuint m_postProcessingProgram;
	GLuint m_programInvertedColor;
	GLuint m_programGreyscale;
	GLuint m_programRainingOnYourScreen;
	GLuint m_programTV_CRT;


	//GLuint m_programReflectiveLighting;
	//GLuint m_programStaticObject;
	//GLuint m_programRimLighting;
	//GLuint m_programDirectionalLighting;
	//GLuint m_programCube;


public:

	CSceneManager() {};
	~CSceneManager() {};

	void InitialSetup();
	void CreatePrograms();
	void DefaultUpdate();

	void Update_Scene1();
	void Render_Scene1();

	void Update_Scene2();
	void Render_Scene2();

	void Update_Scene3();
	void Render_Scene3();


public:

	GLFWwindow* m_pWindow = nullptr;			// Window
	SCENE m_sceneSelector;						// Enum which determine scene selected
	POSTPROCESSING m_postProccessingEffect;		// Enum which determines the post-processing effect applied to scene 3
};

