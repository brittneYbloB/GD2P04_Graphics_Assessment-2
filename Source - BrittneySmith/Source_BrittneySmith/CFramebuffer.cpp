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

#include "CFramebuffer.h"
#include <iostream>

#define WINDOWWIDTH 800
#define WINDOWHEIGHT 800

/***************************************************************************
* CFramebuffer: Create framebuffer
* @parameters:
* @return:
****************************************************************************/
CFramebuffer::CFramebuffer()
{
	// 1) Generate and bind a framebuffer object (FBO)
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);	// GL_FRAMEBUFFER specifies the framebuffer to be read and write (others: GL_DRAW_FRAMEBUFFER, GL_READ_FRAMEBUFFER)

	// 2)
	TextureBinding();
}

/***************************************************************************
* ~CFramebuffer: Deconstructor
* @parameters:
* @return:
****************************************************************************/
CFramebuffer::~CFramebuffer()
{
}

/***************************************************************************
* TextureBinding: Bind a texture
* @parameters:
* @return:
****************************************************************************/
void CFramebuffer::TextureBinding()
{
	// 2) Generate, bind, and create a texture/s
	glGenTextures(1, &m_renderTexture);
	glBindTexture(GL_TEXTURE_2D, m_renderTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOWWIDTH, WINDOWHEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); // match screen size and render across screen (no alpha); null data is populated at runtime
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// 3) Attach the texture/s to the framebuffer object
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,			// Target: Framebuffer type
		GL_COLOR_ATTACHMENT0,	// Attachment type				// Can be altered to create multiple attachments for multiple textures (useful for splitting info, like color, normal or position)
		GL_TEXTURE_2D,			// Texture target type
		m_renderTexture,		// Texture ID
		0);						// Mipmap level

	// 4)
	RenderBufferObject();
}

/***************************************************************************
* RenderBufferObject: Create and attach a Depth/Stencil buffer (RBO used)
* @parameters:
* @return:
****************************************************************************/
void CFramebuffer::RenderBufferObject()
{
	// 4) Create and attach a Depth/Stencil buffer (a RBO can be used!)

	glGenRenderbuffers(1, &m_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);	// only GL_RENDERBUFFER is accepted

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOWWIDTH, WINDOWHEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);	//GL_DEPTH_STENCIL_ATTACHMENT for a combined depth and stencil buffer

	// 5)
	CleanUp();
}

/***************************************************************************
* CleanUp: Check framebuffer status and unbind (clean up)
* @parameters:
* @return:
****************************************************************************/
void CFramebuffer::CleanUp()
{
	// 5) Check framebuffer status and unbind (clean up)
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Error: Framebuffer failed to initialize correctly." << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
