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

#include "CModelLoading.h"

/***************************************************************************
* Mesh_Model: Read the object file and store values. Then, generate a mesh
* @parameters: Folder path and filename
* @return:
****************************************************************************/
void CModel::Mesh_Model(std::string _FolderPath, std::string _FileName)
{
	std::vector<GLuint> Textures;
	std::string FilePath = _FolderPath + _FileName;

	tinyobj::ObjReaderConfig ReaderConfig;
	ReaderConfig.mtl_search_path = ""; // Path to material files

	tinyobj::ObjReader Reader;

	if (!Reader.ParseFromFile(FilePath, ReaderConfig))
	{
		if (!Reader.Error().empty())
		{
			std::cerr << "TinyObjReader: " << Reader.Error();
		}
		exit(1);
	}

	if (!Reader.Warning().empty())
	{
		std::cout << "TinyObjReader: " << Reader.Warning();
	}

	auto& Attrib = Reader.GetAttrib();
	auto& Shapes = Reader.GetShapes();
	auto& Materials = Reader.GetMaterials();

	/*Create the vertices based on read file*/
	
	// Loop through the shapes of the object
	for (size_t ShapeIndex = 0; ShapeIndex < Shapes.size(); ShapeIndex++)
	{
		// Loop through the faces of the shape
		size_t IndexOffset = 0;
		for (size_t FaceIndex = 0; FaceIndex < Shapes[ShapeIndex].mesh.num_face_vertices.size(); FaceIndex++)
		{
			size_t FaceVertexCount = size_t(Shapes[ShapeIndex].mesh.num_face_vertices[FaceIndex]);
			// Loop through the vertices of the faces
			for (size_t VertexIndex = 0; VertexIndex < FaceVertexCount; VertexIndex++)
			{
				VertexFormat Vertex{};
				tinyobj::index_t TinyObjVertex = Shapes[ShapeIndex].mesh.indices[IndexOffset + VertexIndex];
				Vertex.Position = {
					Attrib.vertices[3 * size_t(TinyObjVertex.vertex_index) + 0],
					Attrib.vertices[3 * size_t(TinyObjVertex.vertex_index) + 1],
					Attrib.vertices[3 * size_t(TinyObjVertex.vertex_index) + 2],
				};
				if (TinyObjVertex.normal_index >= 0) // Negative states no Normal data
				{
					Vertex.Normal = {
						Attrib.normals[3 * size_t(TinyObjVertex.normal_index) + 0],
						Attrib.normals[3 * size_t(TinyObjVertex.normal_index) + 1],
						Attrib.normals[3 * size_t(TinyObjVertex.normal_index) + 2],
					};
				}
				if (TinyObjVertex.texcoord_index >= 0) // Negative states no TexCoord data
				{
					Vertex.TexCoord = {
						Attrib.texcoords[2 * size_t(TinyObjVertex.texcoord_index) + 0],
						Attrib.texcoords[2 * size_t(TinyObjVertex.texcoord_index) + 1],
					};
				}
				m_Vertices.push_back(Vertex);
			}
			IndexOffset += FaceVertexCount;

			// per-face material
			GLuint tex = Shapes[ShapeIndex].mesh.material_ids[FaceIndex];
		}
	}

	// Creates the VAO, VBO and vertex attribute pointers
	GenerateMesh();

	/* Load texture using filepath (for only one material) */
	std::string TextureFilePath = _FolderPath + Materials[0].diffuse_texname;
	const char* charTextureFilePath = TextureFilePath.c_str();
	m_texture = CTextureLoader::LoadTexture(charTextureFilePath, m_texture);

	m_p3DTexture = new CTexture3D();
	m_p3DTexture->LoadTexture3D();
}