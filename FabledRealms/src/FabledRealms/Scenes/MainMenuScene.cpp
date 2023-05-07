#include "frpch.h"

#include "Engine/Application.h"

#include "Engine/Input/Input.h"

#include "Engine/Scene/SceneManager.h"
#include "Engine/Sound/AudioManager.h"

#include "MainMenuScene.h"
#include "glad/glad.h"

MainMenuScene::MainMenuScene()
{
	m_MenuScreenVAO = VertexArray::Create();
	m_MenuScreenVAO->Bind();

	float vertices[] {
		// POS              Normal            UV
		 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,     // top right
		 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // bottom right
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,     // bottom left
		-1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,     // top left 
	};

	uint32_t indices[6]{  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};


	m_MenuScreenVBO = VertexBuffer::Create(vertices, sizeof(float) * 32); //Takes in the vertices' size in bytes
	m_MenuScreenIBO = IndexBuffer::Create(indices, 6);


	m_MenuShader = new Shader("Assets/Shaders/MainMenuShader.vert", "Assets/Shaders/MainMenuShader.frag");
	m_MenuShader->setInt("MenuTex", 0);

	const char texturePath[6][100] = {
		"Assets/Textures/FabledRealmsSplash.png",
	};

	m_MenuTexture = new Texture(texturePath, Texture::TEXTURE_TYPE::TEXTURE2D, Texture::TEXTURE_FILTER::LINEAR);

	AudioManager::Get().PlaySound("Assets/Audio/MainMenu01.mp3", true);
	LOG_INFO("CREATED WORLD SCENE");
}

MainMenuScene::~MainMenuScene()
{
	AudioManager::Get().StopAllSounds();

	delete m_MenuScreenVAO;
	delete m_MenuScreenVBO;
	delete m_MenuScreenIBO;
	delete m_MenuShader;
	delete m_MenuTexture;
}

void MainMenuScene::Update(const Time& const time)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	if (Input::IsKeyPressed(KEYCODE_ESCAPE))
	{
		Application::Get().RequestClose();
		return;
	}
	else if (Input::IsKeyPressed(KEYCODE_SPACE))
	{
		SceneManager::Get().SwitchScene(SceneManager::WORLD);
		return;
	}

	// -- RENDERING ------------------------------------
	m_MenuTexture->Bind();
	m_MenuShader->Use();

	Window* window = Application::Get().GetWindow();
	glm::vec2 screenRes = glm::vec2(window->GetWidth(), window->GetHeight());

	m_MenuShader->SetVec2("u_ScreenRes", screenRes);
	m_MenuShader->SetFloat("u_Time", time.currentTime);

	m_MenuScreenVAO->Bind();
	glDrawElements(GL_TRIANGLES, m_MenuScreenIBO->GetCount(), GL_UNSIGNED_INT, 0);
}
