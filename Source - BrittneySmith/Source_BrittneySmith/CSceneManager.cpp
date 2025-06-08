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

#include "CSceneManager.h"

/***************************************************************************
* InitialSetup: Set-up the viewport, time manager, camera class,
*				programs, textures, VAOs, VBOs, EBOs, and enable culling
* @parameter:
* @return:
****************************************************************************/
void CSceneManager::InitialSetup()
{
	srand((unsigned)time(NULL));
	
	// Perlin Noise
	int seed = (int)time(NULL);
	m_perlinNoise = new CPerlinNoise();
	m_perlinNoise->m_Seed = seed;
	m_perlinNoise->SaveToFile();

	// Enabling Depth Testing for 3D
	glEnable(GL_DEPTH_TEST);
	// Declaring a Depth Test function
	glDepthFunc(GL_LESS);	// LESS than the stored depth value

	// Set the colour of the window for when the buffer is cleared
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// (red, green, blue, alpha)

	// Setting up the Viewport
	glViewport(0, 0, 800, 800);

	// Setting up time-manager pointer as is a non-static class
	m_pTimeManager = new CTimeManager();

	// Setting up the camera
	m_pCamera = new CCamera();

	// Light manager
	m_pLightManager = new CLightManager();

	// Create skybox/cubemap
	m_pSkyBox = new CSkyBox(m_pCamera);

	// Input for callback functions
	m_pInputManager = new CInputManager(m_pCamera, m_pTimeManager, m_pLightManager, this);
	m_pInputManager->SetCallbacks(m_pWindow);


	// Enable blending to only render the actual character pixels & no BG
	// Part of the setup for the Per-Fragment Processing
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create programs
	CreatePrograms();

	// Load textures
	m_textureCourage = CTextureLoader::LoadTexture("Images/Courage.jpg", m_textureCourage);

	// Skulls
	m_skullModel.Mesh_Model("Models/Skull/source/ceramicskull_obj/", "ceramicskull.obj");
	m_skullModel.m_objPosition = { 0.0f, 0.0f, 1.0f };

	// Doggie
	m_doggieModel.Mesh_Model("Models/Doggie/", "LucyFullColor.obj");
	m_doggieModel.m_objPosition = { 0.25f, -0.25f, 1.0f };
	m_doggieModel.m_objScale3D = { 0.00075f, 0.00075f, 0.00075f };

	// Normals showing
	m_pSphere = new CSphere(0.75f, 10, m_textureCourage);
	m_pSphereNormals = new CSphere(0.75f, 10, m_textureCourage);
	m_pSphereExplosion = new CSphere(0.75f, 10, m_textureCourage);
	m_pSphere->m_objPosition = glm::vec3(-0.5f, 0.5f, 0.0f);
	m_pSphereNormals->m_objPosition = glm::vec3(-0.5f, 0.5f, 0.0f);
	m_pSphereExplosion->m_objPosition = glm::vec3(0.5f, 0.5f, 0.0f);

	// Tesselation shader/s
	m_trianglePatchTessellationMesh = new CTrianglePatchTessellationMesh();
	m_trianglePatchTessellationMesh->m_objPosition = glm::vec3(-0.5f, -0.75f, 0.0f);
	m_quadPatchTessellationMesh = new CQuadPatchTessellationMesh();
	m_quadTrianglePatcheTessellationMesh = new CQuadFromTrianglePatch();

	// Terrain
	//m_tessHeightMap = new CHeightmapTessellation("Images/heightmap.jpg");

	// Framebuffer Object
	m_framebufferObject = new CFramebuffer();


	// Quad
	m_pQuad = new CQuad();

	// Geometry shader shape test
	m_pointsGeometryShader = new CPointsMesh();


	// Clean up
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// GLOBAL Culling
	glEnable(GL_CULL_FACE);	// Enable face culling globally
	glCullFace(GL_BACK);	// Does not render back-facing polygons
	glFrontFace(GL_CCW);	// Sets the winding order for fron-facing polygons (set to counterclockwise)

	// MSAA
	glfwWindowHint(GLFW_SAMPLES, 4);
	// Enable Multi Sampling
	glEnable(GL_MULTISAMPLE);


	// Hide the cursor and capture it
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


/***************************************************************************
* Update: Update delta time and user input
* @parameter:
* @return:
****************************************************************************/
void CSceneManager::DefaultUpdate()
{
	glfwPollEvents();

	// Text input update
	m_pInputManager->TextCallbackInput(m_pWindow);

	// Calculate delta time
	m_pTimeManager->CalculateDeltaTime();

	// Calculate camera projection matrix
	m_pCamera->CalculateProjectionMatrix();

	// Calculate camera view matrix
	m_pCamera->CalculateViewMatrix();

	// Update SkyBox
	m_pSkyBox->Update(m_pTimeManager->m_fDeltaTime);
}


/***************************************************************************
* Update_Scene1: Geometry Shader
* @parameter:
* @return:
****************************************************************************/
void CSceneManager::Update_Scene1()
{
	m_pointsGeometryShader->UpdateObject(0.0f, glm::vec3(0, 1, 0), m_pTimeManager->m_fDeltaTime);

	// Normals showing
	//m_skullModel.UpdateObject(0.0f, glm::vec3(0, 1, 0), m_pTimeManager->m_fDeltaTime);
	m_pSphere->UpdateObject(0.0f, glm::vec3(0, 1, 0), m_pTimeManager->m_fDeltaTime);
	m_pSphereNormals->UpdateObject(0.0f, glm::vec3(0, 1, 0), m_pTimeManager->m_fDeltaTime);

	// Explosion!
	m_doggieModel.UpdateObject(0.0f, glm::vec3(0, 1, 0), m_pTimeManager->m_fDeltaTime);
	m_pSphereExplosion->UpdateObject(0.0f, glm::vec3(0, 1, 0), m_pTimeManager->m_fDeltaTime);
}

/***************************************************************************
* Update_Scene2: Tessellation Shaders + Level of Detail (LOD)
* @parameter:
* @return:
****************************************************************************/
void CSceneManager::Update_Scene2()
{
	m_trianglePatchTessellationMesh->UpdateObject(0.0f, glm::vec3(0, 1, 0), m_pTimeManager->m_fDeltaTime);

	m_quadTrianglePatcheTessellationMesh->UpdateObject(0.0f, glm::vec3(0, 1, 0), m_pTimeManager->m_fDeltaTime);

	//m_tessHeightMap->UpdateObject();
}

/***************************************************************************
* Update_Scene3: Framebuffers + Post-Processing
* @parameter:
* @return:
****************************************************************************/
void CSceneManager::Update_Scene3()
{
	// 3D objects
	m_skullModel.UpdateObject(0.0f, glm::vec3(0, 1, 0), m_pTimeManager->m_fDeltaTime);
	m_doggieModel.UpdateObject(0.0f, glm::vec3(0, 1, 0), m_pTimeManager->m_fDeltaTime);
	m_pSphere->UpdateObject(0.0f, glm::vec3(0, 1, 0), m_pTimeManager->m_fDeltaTime);

	// Update post processing
	switch (m_postProccessingEffect)
	{
	case POSTPROCESSING::e_NONE:
		m_postProcessingProgram = CShaderLoader::CreateProgram("Shaders/2DShape.vs", "Shaders/StaticTexture.frag");
		break;
	case POSTPROCESSING::e_COLORINVERSION:
		m_postProcessingProgram = m_programInvertedColor;
		break;
	case POSTPROCESSING::e_GREYSCALE:
		m_postProcessingProgram = m_programGreyscale;
		break;
	case POSTPROCESSING::e_RAIN:
		m_postProcessingProgram = m_programRainingOnYourScreen;
		break;
	case POSTPROCESSING::e_TVCRT:
		m_postProcessingProgram = m_programTV_CRT;
		break;
	default:
		break;
	}
}





/***************************************************************************
* Render_Scene1: Geometry Shader
* @parameter:
* @return:
****************************************************************************/
void CSceneManager::Render_Scene1()
{
	// Skybox
	m_pSkyBox->Render(m_programSkyBox, m_pCamera);

	// Stars
	m_pointsGeometryShader->Render(m_programGeometryShader, m_pCamera, m_pLightManager);

	// Normals showing
	//m_skullModel.Render(m_programGeometryShaderShowingNormals, m_pCamera, m_pLightManager);
	m_pSphere->Render(m_programCombinedLights, m_pCamera, m_pLightManager);
	m_pSphereNormals->Render(m_programGeometryShaderShowingNormals, m_pCamera, m_pLightManager);
	
	// Explosion!
	m_doggieModel.Render(m_programGeometryShaderExplosion, m_pCamera, m_pLightManager);
	m_pSphereExplosion->Render(m_programGeometryShaderExplosion, m_pCamera, m_pLightManager);
}

/***************************************************************************
* Render_Scene2: Tessellation Shaders + Level of Detail (LOD)
* @parameter:
* @return:
****************************************************************************/
void CSceneManager::Render_Scene2()
{
	// Skybox
	m_pSkyBox->Render(m_programSkyBox, m_pCamera);

	//
	m_trianglePatchTessellationMesh->Render(m_programTrianglePatch_TS, m_pCamera, m_pLightManager);

	m_quadTrianglePatcheTessellationMesh->Render(m_programTrianglePatch_TS, m_pCamera, m_pLightManager);

	//
	//m_tessHeightMap->Render(m_programLitHeightmap, m_pCamera, m_pLightManager);
}

/***************************************************************************
* Render_Scene3: Framebuffers + Post-Processing
* @parameter:
* @return:
****************************************************************************/
void CSceneManager::Render_Scene3()
{
	// Clear the color, depth and stencil bits after binding and before rendering
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferObject->m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear stencil buffer too if used!

	/* Render the scene */
	// Skybox
	m_pSkyBox->Render(m_programSkyBox, m_pCamera);

	// Objects (3 MINIMUM)
	m_skullModel.Render(m_programCombinedLights, m_pCamera, m_pLightManager);
	m_doggieModel.Render(m_programCombinedLights, m_pCamera, m_pLightManager);
	m_pSphere->Render(m_programCombinedLights, m_pCamera, m_pLightManager);

	/******************/

	// Unbind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Render a full-window quad using the framebuffer's colour texture
	m_pQuad->Render(m_postProcessingProgram, m_framebufferObject->m_renderTexture, m_pTimeManager->GetCurrentTimeStep());	// <-- this object needs access to the color attachment texture of the FBO!
}



/***************************************************************************
* CreatePrograms: Create/set programs
* @parameter:
* @return:
****************************************************************************/
void CSceneManager::CreatePrograms()
{
	// Create lighting programs
	m_programCombinedLights = CShaderLoader::CreateProgram("Shaders/3D_Normal.vs", "Shaders/CombinedLighting.frag");
	m_programLitHeightmap = CShaderLoader::CreateProgram("Shaders/3D_Normal.vs", "Shaders/LitHeightmap.frag");
	m_program3DTexture = CShaderLoader::CreateProgram("Shaders/3D_Normal.vs", "Shaders/Fog.frag");
	m_programSkyBox = CShaderLoader::CreateProgram("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");
	m_programSkyBoxFog = CShaderLoader::CreateProgram("Shaders/SkyBox.vs", "Shaders/SkyboxFog.frag");
	// Geometry shader program
	m_programGeometryShader = CShaderLoader::CreateProgram_VGF("Shaders/GeoVertex.vert", "Shaders/Triangle.geo", "Shaders/StaticTexture.frag");
	m_programGeometryShaderShowingNormals = CShaderLoader::CreateProgram_VGF("Shaders/GeoVertexShowingNormals.vert", "Shaders/TriangleShowingNormals.geo", "Shaders/FixedColor.frag");
	m_programGeometryShaderExplosion = CShaderLoader::CreateProgram_VGF("Shaders/GeoVertexShowingNormals.vert", "Shaders/TriangleExplosionEffect.geo", "Shaders/StaticTexture.frag");
	// Tessallation programs
	m_programQuadPatch_TS = CShaderLoader::CreateProgram_VTF("Shaders/TessellationVS.vs", "Shaders/TCS_QuadPatch.tcs", "Shaders/TES_QuadPatch.tes", "Shaders/FixedColor.frag");
	m_programTrianglePatch_TS = CShaderLoader::CreateProgram_VTF("Shaders/TessellationVS.vs", "Shaders/TCS_TrianglePatch.tcs", "Shaders/TES_TrianglePatch.tes", "Shaders/StaticTexture.frag"); // FixedColor
	m_programTerrain_TS = CShaderLoader::CreateProgram_VTF("Shaders/3D_Normal.vs", "Shaders/TCS_TrianglePatch.tcs", "Shaders/TES_TrianglePatch.tes", "Shaders/LitHeightmap.frag");
	// Post processprograms
	m_programInvertedColor = CShaderLoader::CreateProgram("Shaders/2DShape.vs", "Shaders/InvertColor.frag");
	m_programGreyscale = CShaderLoader::CreateProgram("Shaders/2DShape.vs", "Shaders/Greyscale.frag");
	m_programRainingOnYourScreen = CShaderLoader::CreateProgram("Shaders/2DShape.vs", "Shaders/RainingOnYourScreen.frag");
	m_programTV_CRT = CShaderLoader::CreateProgram("Shaders/2DShape.vs", "Shaders/TV_CRT_Pixels.frag");
}


