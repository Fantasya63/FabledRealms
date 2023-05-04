#pragma once
#include <Engine/Scene/Scene.h>
#include "Engine/Graphics/Shader.h"
#include "Engine/Graphics/Buffers.h"
#include "Engine/Graphics/VertexArray.h"
#include "Engine/Graphics/Texture.h"

#include "Engine/Graphics/Camera.h"


class MainMenuScene : public Scene
{
public:
	MainMenuScene();
	~MainMenuScene();
	virtual void Update(const Time& const time) override;

private:
	VertexArray* m_MenuScreenVAO;
	VertexBuffer* m_MenuScreenVBO;
	IndexBuffer* m_MenuScreenIBO;

	Shader* m_MenuShader;
	Texture* m_MenuTexture;
};
