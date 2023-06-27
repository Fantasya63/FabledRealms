#include "frpch.h"
#include "Mesh.h"
#include <glad/glad.h>
#include <Engine/Graphics/Mesh/Vertex.h>

Mesh::~Mesh()
{
	FR_CORE_ASSERT(!m_IsInitialized, "Attempting to destroy mesh without calling cleanup! Causes Memory Leak!");
}

void Mesh::RenderMesh(Shader& shader)
{
	FR_CORE_ASSERT(VAO, "Mesh VAO is not Initialized!");
	FR_CORE_ASSERT(VBO, "Mesh VBO is not Initialized!");
	FR_CORE_ASSERT(IBO, "Mesh EBO is not Initialized!");


	//Setup Shader
	shader.SetInt("DiffuseTex", 0);
	shader.SetInt("RoughnessTex", 1);
	shader.SetInt("NormalTex", 2);
	shader.SetInt("MetallicTex", 3);
	shader.SetInt("CubemapTex", 4);


	// Textures
	if (DiffuseTexID)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, DiffuseTexID);
	}
	
	if (RoughnessTexID)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, RoughnessTexID);
	}

	if (NormalTexID)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, NormalTexID);
	}

	if (MetallicTexID)
	{
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, MetallicTexID);
	}

	if (CubemapTexID)
	{
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, CubemapTexID);
	}

	if (diffuseIrradianceTexID)
	{
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_CUBE_MAP, diffuseIrradianceTexID);
	}
	
	if (prefilteredMapTexID)
	{
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_CUBE_MAP, prefilteredMapTexID);
	}
	
	if (brdfLUTTexID)
	{
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, brdfLUTTexID);
	}



	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, NumOfIndices, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}


// --------------------------------

void InitMesh(Mesh& mesh)
{
	FR_CORE_ASSERT(!mesh.m_IsInitialized, "Mesh is already Initialized!");
	mesh.m_IsInitialized = true;

	glGenVertexArrays(1, &mesh.VAO);
	glGenBuffers(1, &mesh.VBO);
	glGenBuffers(1, &mesh.IBO);

	glBindVertexArray(mesh.VAO);
}

void SendData(Mesh& mesh, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
	// Send Data
	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

	mesh.NumOfIndices = indices.size();

	// Set Vertex Layouts
	//POSITION
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	//UV1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV1));

	//NORMAL
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	//TANGENT
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

	//AO
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, AO));
}

void Mesh::CleanUpMesh(Mesh& mesh)
{
#ifdef FR_DEBUG
	FR_CORE_ASSERT(mesh.m_IsInitialized, "Attempting to cleanup Uninitialized Mesh!");
	mesh.m_IsInitialized = false;
#endif // FR_DEBUG


	glDeleteVertexArrays(1, &mesh.VAO);
	glDeleteBuffers(1, &mesh.VBO);
	glDeleteBuffers(1, &mesh.IBO);
}

glm::vec3 Mesh::GetTangentVector(const std::vector<Vertex>& vertices)
{
	//From https://learnopengl.com/Advanced-Lighting/Normal-Mapping

	glm::vec3 edge1 = vertices[1].Position - vertices[0].Position;
	glm::vec3 edge2 = vertices[2].Position - vertices[0].Position;
	glm::vec2 deltaUV1 = vertices[1].UV1 - vertices[0].UV1;
	glm::vec2 deltaUV2 = vertices[2].UV1 - vertices[0].UV1;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	glm::vec3 tangent;
	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

	return tangent;
}

void Mesh::InitMeshFullScreenQuad(Mesh& mesh)
{
	InitMesh(mesh);

	//Quad data
	const std::vector<Vertex> vertices = {
				// POS                 UV1			  Normal              Tangent		AO
		
		//Top Left
		{
			{ -1.0f,  1.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0,
		},

		//Bottom Left
		{
			{ -1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0,
		},

		//Bottom Right
		{
			{  1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0
		},

		//Top Right
		{
			{  1.0f,  1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0
		},
	};

	const std::vector<uint32_t> indices = {
		0, 1, 2,  // Top Left Triangle
		0, 2, 3   // Bottom Right Triangle
	};


	SendData(mesh, vertices, indices);

	//Unbind VAO
	glBindVertexArray(0);
}

void Mesh::InitMeshCubemap(Mesh& mesh)
{
	InitMesh(mesh);

	//Data

	//Vertex Data for Skybox
	const std::vector<Vertex> vertices = {

		// Positions				UV1				Normal				Tangent            
		{ { -1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 0},  //Back Top Left   [0]
		{ {  1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 0},  //Back Top Right  [1]
		{ {  1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 0},  //Back Bot Right  [2]
		{ { -1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 0},  //Back Bot Left   [3]
		{ { -1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 0},  //Front Top Left  [4]
		{ {  1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 0},  //Front Top Right [5]
		{ {  1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 0},  //Front Bot Right [6]
		{ { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 0},  //Front Bot Left  [7]
	};

	// Index of the vertices for the skybox
	// Refer to skyboxVertices Indices
	const std::vector<uint32_t> indices =
	{
		//Back Face
		0, 2, 3,
		0, 1, 2,

		//Front Face
		4, 7, 6,
		4, 6, 5,

		//Left
		4, 3, 7,
		4, 0, 3,

		//Right
		1, 6, 2,
		1, 5, 6,

		//Top
		4, 1, 0,
		4, 5, 1,

		//Bottom
		7, 3, 2,
		7, 2, 6
	};

	SendData(mesh, vertices, indices);

	glBindVertexArray(0);
}

void Mesh::InitMeshChunk(Mesh& mesh, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
	InitMesh(mesh);

	//Upload Data
	SendData(mesh, vertices, indices);

	glBindVertexArray(0);
}
