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
#include <iostream>
#include <fstream>
#include <sstream> // for getline

class CShaderLoader
{
public:
	// Creating the Shaders (! Perform this for each shader !)
	static GLuint CreateShader(GLenum shaderType, std::string shaderFilename);

	// Creating the Program
	static GLuint CreateProgram(std::string _VertexShaderName, std::string _FragShaderName);
	static GLuint CreateProgram_VGF(std::string _VertexShaderName, std::string _Geometry, std::string _FragShaderName);
	static GLuint CreateProgram_VTF(std::string _VertexShaderName, std::string _TCS, std::string _TES, std::string _FragShaderName);
	
	// File reading/string storage
	static std::string ReadTextFile(std::string shaderFilename);
};