#pragma once

#include <Engine/Graphics/Texture.h>
#include <Engine/Graphics/Shader.h>
#include "Vertex.h"

struct Mesh
{
public:
	~Mesh();

	uint32_t VAO;
	uint32_t VBO;
	uint32_t IBO;

	uint32_t NumOfIndices;

	//Texture slots
	uint32_t DiffuseTexID   = 0;
	uint32_t RoughnessTexID = 0;
	uint32_t NormalTexID    = 0;
	uint32_t MetallicTexID  = 0;

	uint32_t CubemapTexID   = 0;
	uint32_t diffuseIrradianceTexID = 0;
	uint32_t prefilteredMapTexID = 0;
	uint32_t brdfLUTTexID = 0;

	bool m_IsInitialized = false;

public:
	void RenderMesh(Shader& shader);
	
	// Deletes the VAO, VBO and IBO in the GPU and sets IsInitialized to false
	static void CleanUpMesh(Mesh& mesh);

	static glm::vec3 GetTangentVector(const std::vector<Vertex>& verts);


	//Initializes the mesh to a Fullscreen Quad
	static void InitMeshFullScreenQuad(Mesh& mesh);

	//Initializes the mesh to a Cubemap mesh
	static void InitMeshCubemap(Mesh& mesh);

	static void InitMeshWaterPlane(Mesh& mesh);

	//Intialized the mesh based on the given vertices and indices
	static void InitMeshChunk(Mesh& mesh, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

};

