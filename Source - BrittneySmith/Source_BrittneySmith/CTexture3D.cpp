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

#include "CTexture3D.h"

/***************************************************************************
* CTexture3D: Generate texture
* @parameters:
* @return: 
****************************************************************************/
CTexture3D::CTexture3D()
{
	Data3D = new GLubyte[TextureWidth * TextureHeight * TextureDepth * 4];	// Extra number (*4) at the end as each element is a byte; RGBA channels = 4 bytes

	GeneratePattern3D();
	GenerateData3D();
	LoadTexture3D();
}

/***************************************************************************
* GeneratePattern3D: Generate a 3D pattern
* @parameters:
* @return:
****************************************************************************/
void CTexture3D::GeneratePattern3D()
{
	for (int X = 0; X < TextureWidth; X++)
	{
		for (int Y = 0; Y < TextureHeight; Y++)
		{
			for (int Z = 0; Z < TextureDepth; Z++)
			{
				/* Pattern needs some form of a condition to have a variable value
				TexturePattern3D[X][Y][Z] = Value */
				TexturePattern3D_Striped[X][Y][Z] = (Y / 20) % 2 ? 0 : 1;	// Pattern of stripes alternating through the Y-axis

				if (!m_bIsStripedOnly)
				{
					TexturePattern3D_Checkerboard[X][Y][Z] = (X / 20) % 2 ? 0 : 1;
				}
			}
		}
	}
}


/***************************************************************************
* GenerateData3D: Fill a byte array with Color (RGBA) values using the 3D pattern
* @parameters:
* @return:
****************************************************************************/
void CTexture3D::GenerateData3D()
{
	for (int X = 0; X < TextureWidth; X++)
	{
		for (int Y = 0; Y < TextureHeight; Y++)
		{
			for (int Z = 0; Z < TextureDepth; Z++)
			{
				int Index = X * (TextureWidth * TextureHeight * 4) + Y * (TextureHeight * 4) + Z * 4;
			
				if (TexturePattern3D_Striped[X][Y][Z] == 1)
				{
					// Red Color
					Data3D[Index + 0] = (GLubyte)255;	// Red
					Data3D[Index + 1] = (GLubyte)0;		// Green
					Data3D[Index + 2] = (GLubyte)0;		// Blue
					Data3D[Index + 3] = (GLubyte)255;	// Alpha
				}
				else
				{
					// White Color
					Data3D[Index + 0] = (GLubyte)255;	// Red
					Data3D[Index + 1] = (GLubyte)255;	// Green
					Data3D[Index + 2] = (GLubyte)255;	// Blue
					Data3D[Index + 3] = (GLubyte)255;	// Alpha
				}
			}
		}
	}
}

/***************************************************************************
* LoadTexture3D: Create the 3D texture using the byte array
* @parameters:
* @return:
****************************************************************************/
void CTexture3D::LoadTexture3D()
{
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_3D, TextureID);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexStorage3D(GL_TEXTURE_3D, 1, GL_RGBA8, TextureWidth, TextureHeight, TextureDepth);
	glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, TextureWidth, TextureHeight, TextureDepth,
		GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, Data3D);
}

/***************************************************************************
* Render: Render
* @parameters: CModelMesh* _Mesh, GLuint _Program, CCamera* _Camera, CLightManager* _lightManager, float _ModelDependentValue
* @return:
****************************************************************************/
void CTexture3D::Render(CModelMesh* _Mesh, GLuint _Program, CCamera* _Camera, CLightManager* _lightManager, float _ModelDependentValue)
{
	glUseProgram(_Program);
	glBindVertexArray(_Mesh->m_VAO);

	// Pass in camera position via uniform for specular lighting
	glUniform3fv(glGetUniformLocation(_Program, "CameraPos"), 1, glm::value_ptr(_Camera->m_cameraPos));

	glUniform1f(glGetUniformLocation(_Program, "ModelDependentValue"), _ModelDependentValue);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, TextureID);
	glUniform1i(glGetUniformLocation(_Program, "Texture3D"), 0);

	// Standard Render() operations
	_Mesh->m_is3DTextured = true;
	_Mesh->Render(_Program, _Camera, _lightManager);

	glBindTexture(GL_TEXTURE_3D, 0);
	glUseProgram(0);
}
