#pragma once
#include <Engine/Scene/Scene.h>
#include "Engine/Graphics/Shader.h"
#include "Engine/Graphics/Texture.h"
#include <Engine/Graphics/Mesh/Mesh.h>

#include "Engine/Graphics/Camera.h"


class MainMenuScene : public Scene
{
public:
	MainMenuScene();
	~MainMenuScene();
	virtual void Update(const Time& time) override;

private:
	Mesh m_ScreenQuadMesh;

	Shader* m_MenuShader;
	Texture m_MenuTexture;
};
