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

#include "CShaderLoader.h"

/***************************************************************************
* CreateShader: Read the shader file, create shader (based on shader
                type), and compile
* @parameters:  Shader type (.vs or .frag) and shader filename/filepath
* @return:	    Shader ID
****************************************************************************/
GLuint CShaderLoader::CreateShader(GLenum shaderType, std::string shaderFilename)
{
	// Read the text file in entirety and load into a string variable (source code)
	std::string shaderSourceCode = ReadTextFile(shaderFilename);

	//	• Assign a GLuint variable for the shader ID. 
	GLuint m_ShaderID = glCreateShader(shaderType); // Returns: Shader ID in the form of a GLuint.

	//	• Create the Shader using source code read from the text file.
		// --> shader : Specifies the ID of the shader to populate.
		// --> count : Specifies the number of elements in the string and length arrays.
		// --> string : Specifies an array of pointers containing the source code.
	const GLchar* constString = shaderSourceCode.c_str();
	// --> length : Specifies an array of string lengths.
	const GLint constLength = shaderSourceCode.size();

	// Create
	glShaderSource(m_ShaderID, 1, &constString, &constLength);

	//	• Compile the shader.
	glCompileShader(m_ShaderID);


	return GLuint(m_ShaderID);
}


/***************************************************************************
* CreateProgram: Create the program using the specifed vertex and fragment shaders
* @parameters:   Vertex and fragment shaders
* @return:	     Program ID
****************************************************************************/
GLuint CShaderLoader::CreateProgram(std::string _VertexShaderName, std::string _FragShaderName)
{
	// Create the shaders
	//GLuint vertexShaderID = CShaderLoader::CreateShader(GL_VERTEX_SHADER, "FixedTriangle.vs");
	GLuint vertexShaderID = CShaderLoader::CreateShader(GL_VERTEX_SHADER, _VertexShaderName);
	GLuint fragmentShaderID = CShaderLoader::CreateShader(GL_FRAGMENT_SHADER, _FragShaderName);

	// Assign a GLuint variable for the program ID.
	GLuint m_ProgramID = glCreateProgram();

	//	• Attach the vertex shader.
	glAttachShader(m_ProgramID, vertexShaderID);
	// --> program : The program ID of the program to attach the shader to.
	// --> shader : The ID of the shader to be attached.
	// --> This needs to be called for both shaders(twice at this point; see below)!!

	//	• Attach the fragment shader.
	glAttachShader(m_ProgramID, fragmentShaderID);

	//	• Link the program.
	glLinkProgram(m_ProgramID);
	// --> shader : Specifies the ID of the shader to compile.


	return GLuint(m_ProgramID);
}


/***************************************************************************
* CreateProgram_VGF:
* @parameters:   std::string _VertexShaderName, std::string _Geometry, std::string _FragShaderName
* @return:	     Program ID
****************************************************************************/
GLuint CShaderLoader::CreateProgram_VGF(std::string _VertexShaderName, std::string _Geometry, std::string _FragShaderName)
{
	// Create the shaders
	//GLuint vertexShaderID = CShaderLoader::CreateShader(GL_VERTEX_SHADER, "FixedTriangle.vs");
	GLuint vertexShaderID = CShaderLoader::CreateShader(GL_VERTEX_SHADER, _VertexShaderName);
	GLuint fragmentShaderID = CShaderLoader::CreateShader(GL_FRAGMENT_SHADER, _FragShaderName);
	GLuint geometryShaderID = CShaderLoader::CreateShader(GL_GEOMETRY_SHADER, _Geometry);

	// Assign a GLuint variable for the program ID.
	GLuint m_ProgramID = glCreateProgram();

	//	• Attach the vertex shader.
	glAttachShader(m_ProgramID, vertexShaderID);
	// --> program : The program ID of the program to attach the shader to.
	// --> shader : The ID of the shader to be attached.
	// --> This needs to be called for both shaders(twice at this point; see below)!!

	// Attach geometry shader
	glAttachShader(m_ProgramID, geometryShaderID);

	//	• Attach the fragment shader.
	glAttachShader(m_ProgramID, fragmentShaderID);

	//	• Link the program.
	glLinkProgram(m_ProgramID);
	// --> shader : Specifies the ID of the shader to compile.


	return GLuint(m_ProgramID);
}

GLuint CShaderLoader::CreateProgram_VTF(std::string _VertexShaderName, std::string _TCS, std::string _TES, std::string _FragShaderName)
{
	// Create the shaders
	//GLuint vertexShaderID = CShaderLoader::CreateShader(GL_VERTEX_SHADER, "FixedTriangle.vs");
	GLuint vertexShaderID = CShaderLoader::CreateShader(GL_VERTEX_SHADER, _VertexShaderName);
	GLuint TCSshaderID = CShaderLoader::CreateShader(GL_TESS_CONTROL_SHADER, _TCS);
	GLuint TESshaderID = CShaderLoader::CreateShader(GL_TESS_EVALUATION_SHADER, _TES);
	GLuint fragmentShaderID = CShaderLoader::CreateShader(GL_FRAGMENT_SHADER, _FragShaderName);

	// Assign a GLuint variable for the program ID.
	GLuint m_ProgramID = glCreateProgram();

	//	• Attach the vertex shader.
	glAttachShader(m_ProgramID, vertexShaderID);
	// --> program : The program ID of the program to attach the shader to.
	// --> shader : The ID of the shader to be attached.
	// --> This needs to be called for both shaders(twice at this point; see below)!!

	// Attach geometry shader
	glAttachShader(m_ProgramID, TCSshaderID);
	glAttachShader(m_ProgramID, TESshaderID);

	//	• Attach the fragment shader.
	glAttachShader(m_ProgramID, fragmentShaderID);

	//	• Link the program.
	glLinkProgram(m_ProgramID);
	// --> shader : Specifies the ID of the shader to compile.


	return GLuint(m_ProgramID);
}


/***************************************************************************
* ReadTextFile: Read text file
* @parameters:  Filename / filepath
* @return:	    Characters from file stream stored in a string variable
****************************************************************************/
std::string CShaderLoader::ReadTextFile(std::string shaderFilename)
{
	// Open stream to file
	std::ifstream InputFile(shaderFilename);

	// In event of failure to access streaming file, display error message
	if (InputFile.fail())
	{
		std::cerr << "\nError: Could not open requested file." << "\n\n";
	}

	// Retrieve characters from file stream and store into string variable
	return std::string((std::istreambuf_iterator<char>(InputFile)), std::istreambuf_iterator<char>());
}
