#pragma once
#include "Engine/Scene/Scene.h"


#include "Engine/Graphics/Buffers.h"
#include "Engine/Graphics/VertexArray.h"
#include "Engine/Graphics/Shader.h"
#include "Engine/Graphics/Texture.h"
#include "Engine/Graphics/Camera.h"

#include "FabledRealms/Chunk/World.h"

class WorldScene : public Scene
{
public:
	WorldScene();
	~WorldScene();

	virtual void Update(const Time& const time) override;

private:
	// ----------- Cubemap -----------

	VertexArray* m_CubemapVAO;
	VertexBuffer* m_CubemapVBO;
	IndexBuffer* m_CubemapIBO;
	Shader* m_CubemapShader;
	Texture* m_CubemapTexture;

	// ------------ World ---------------
	World m_World;

	Shader* m_Shader;
	Texture* m_Texture;

	Camera m_Camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f));
};

