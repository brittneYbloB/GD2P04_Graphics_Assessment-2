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

#include "CTextureLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

/***************************************************************************
* LoadTexture: Load texture by loading the file, binding, mipmaps, and populating
*			   the texture with the loaded image's data
* @parameters: The image file name and designated texture to populate
* @return:	   Texture
****************************************************************************/
GLuint CTextureLoader::LoadTexture(const char* _TextureFile, GLuint _Texture)
{
	// Load the Image data
	int ImageWidth;
	int ImageHeight;
	int ImageComponents;

	stbi_set_flip_vertically_on_load(true);	// flips image to correct orientation

	// Load the file (png/jpg string name)
	unsigned char* ImageData = stbi_load(_TextureFile,
		&ImageWidth, &ImageHeight, &ImageComponents, 0);

	/* Generating a Texture ID  // Create and bind a new texture template */
	glGenTextures(1, &_Texture);
	glBindTexture(GL_TEXTURE_2D, _Texture);

	// Check how many components the loaded image has (RGB or RGBA?)
	GLint LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

	// Populate the texture with the image data
	glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0,
		LoadedComponents, GL_UNSIGNED_BYTE, ImageData);

	// Setting the filtering and mipmap parameters for this texture // MUST BE SET FOR EVERY TEXTURE INDIVIDUALLY
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Setting the address mode for this texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// below is code for the address mode Clamp to Border, and would replace the above 2 lines. Uses suffix 'fv' instead of 'i'
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, BorderColor);

	// Generate the mipmaps, free the memory and unbind the texture
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(ImageData);
	glBindTexture(GL_TEXTURE_2D, 0);


	return _Texture;
}
