#pragma once
#include "Engine/Scene/Scene.h"

#include <Engine/Graphics/Mesh/Mesh.h>
#include "Engine/Graphics/Shader.h"
#include "Engine/Graphics/Texture.h"

#include <Engine/Graphics/Rendering/GeometryBuffer.h>
#include <Engine/Graphics/Post Processing/HdrFBO.h>
#include <Engine/Graphics/Post Processing/BloomFBO.h>

#include "Engine/Graphics/Camera.h"
#include  "FabledRealms/World/World.h"

class WorldScene : public Scene
{
public:
	WorldScene();
	~WorldScene();

	virtual void Update(const Time& time) override;
	virtual void OnWindowResized(int width, int height) override;

private:
	// Frame Buffers
	GeometryBuffer* m_GeometryBuffer;
	HdrFBO* m_HDRBufffer;
	BloomFBO* m_BloomFBO;



	// Shaders
	Shader* m_TonemappingShader;
	Shader* m_GeometryBufferShader;
	Shader* m_DefferedLightingShader;


	// ----------- Crosshair ----------
	Mesh m_CrosshairMesh;

	Shader* m_CrosshairShader;
	Texture m_CrosshairTexture;



	// ----------- Cubemap -----------

	Mesh m_CubemapMesh;
	Shader* m_CubemapShader;
	Texture m_CubemapTexture;
	Texture m_DiffuseIrradianceTexture;


	// ------------ World ---------------
	World m_World;
	Camera m_Camera = Camera(glm::vec3(0.0f, 55.0f, 0.0f));
};

