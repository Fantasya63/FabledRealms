#pragma once
#include "Engine/Scene/Scene.h"

#include <Engine/Graphics/Mesh/Mesh.h>
#include "Engine/Graphics/Shader.h"
#include "Engine/Graphics/Texture.h"

#include <Engine/Graphics/Rendering/GeometryBuffer.h>
#include <Engine/Graphics/Rendering/ShadowFBO.h>
#include <Engine/Graphics/Post Processing/HdrFBO.h>
#include <Engine/Graphics/Post Processing/BloomFBO.h>
#include <Engine/Graphics/Post Processing/SSR/ScreenSpaceReflectionFBO.h>

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
	ShadowFBO* m_ShadowFBO;
	ScreenSpaceReflectionFBO* m_SSRFBO;

	// Shaders
	Shader* m_TonemappingShader;
	Shader* m_GeometryBufferShader;
	Shader* m_DefferedLightingShader;
	Shader* m_ShadowMapShader;
	Shader* m_WaterPlaneShader;

	// ----------- Crosshair ----------
	Mesh m_CrosshairMesh;

	Shader* m_CrosshairShader;
	Texture m_CrosshairTexture;



	// ----------- Cubemap -----------

	Mesh m_CubemapMesh;
	Mesh m_WaterPlaneMesh;
	Shader* m_CubemapShader;
	Texture m_CubemapTexture;
	Texture m_DiffuseIrradianceTexture;
	Texture m_prefilteredTexture;
	Texture m_brdfTexture;
	Texture m_BakedBRDFTexture;
	Texture m_WaterNormalTexture;


	// ------------ World ---------------
	World m_World;
	Camera m_Camera = Camera(glm::vec3(0.0f, 55.0f, 0.0f));
};

