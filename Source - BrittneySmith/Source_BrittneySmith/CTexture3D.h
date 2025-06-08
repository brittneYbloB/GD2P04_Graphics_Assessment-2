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
// Library Includes
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include "CModelMesh.h"

class CTexture3D 
	: public CModelMesh
{
private:
	GLubyte* Data3D;
	GLuint TextureID;

	static const int TextureWidth = 200;
	static const int TextureHeight = 200;
	static const int TextureDepth = 200;
	int TexturePattern3D_Striped[TextureWidth][TextureHeight][TextureDepth];
	int TexturePattern3D_Checkerboard[TextureWidth][TextureHeight][TextureDepth];


public:
	CTexture3D();
	~CTexture3D() { delete Data3D; };

	void GeneratePattern3D();
	void GenerateData3D();
	void LoadTexture3D();

	void Render(CModelMesh* _Mesh, GLuint _Program, CCamera* _Camera, CLightManager* _lightManager, float _ModelDependentValue);


public:
	bool m_bIsStripedOnly = false;


	//void glTexStorage3D(
	//	GLenum _target,				// Target to which the texture is bound (GL_TEXTURE_3D)
	//	GLsizei _levels,			// The number of texture levels
	//	GLenum _internalFormat,		// Internal format to store the texture data - e.g. GL_RGBAB
	//	GLsizei _width,				// Width of the texture, in texels
	//	GLsizei _height,			// Height of the texture, in texels
	//	GLsizei _depth				// Depth of the texture, in texels
	//) {};

	//void glTexSubImage3D(
	//	GLenum _target,				// Target to which the texture is bound (GL_TEXTURE_3D)
	//	GLint _level,				// Level of detail (mipmaps). Level 0 is base image
	//	GLint _xoffset,				// Texel offset in the X direction within the texture array
	//	GLint _yoffset,				// Texel offset in the Y direction within the texture array
	//	GLint _zoffset,				// Texel offset in the Z direction within the texture array
	//	GLsizei _width,				// Width of the texture subimage
	//	GLsizei _height,			// Height of the texture subimage
	//	GLsizei _depth,				// Depth of the texture subimage
	//	GLenum _format,				// Data format of the pixel data - e.g. GL_RED, GL_RGP, GL_RGBA, etc.
	//	GLenum _type,				/* Data type of the pixel data in order due to endianess (eg. 8, 16, 32 bit forward / reverse) */

	//	const void* _pixels			// Pointer to the data (Byte Stream)
	//) {};
};

