#include "frpch.h"
#include "Engine/Input/Input.h"

#include "Engine/Scene/SceneManager.h"
#include "Engine/Sound/AudioManager.h"

#include "MainMenuScene.h"
#include "glad/glad.h"

MainMenuScene::MainMenuScene()
{
	m_MenuScreenVAO = VertexArray::Create();
	m_MenuScreenVAO->Bind();

	float* vertices = new float[20] {
		// POS                  UV
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,     // top right
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,     // bottom right
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,     // bottom left
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,     // top left 
	};

	uint32_t* indices = new uint32_t[6]{  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};


	m_MenuScreenVBO = VertexBuffer::Create(vertices, sizeof(float) * 20);
	m_MenuScreenIBO = IndexBuffer::Create(indices, 6);


	AudioManager::Get().PlaySound("Assets/Audio/MainMenu01.wav", true);
	LOG_INFO("CREATED WORLD SCENE");
}

MainMenuScene::~MainMenuScene()
{
	LOG_CORE_INFO("Destructior Main Menu");
	AudioManager::Get().StopAllSounds();
}

void MainMenuScene::Update(const Time& const time)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (Input::IsKeyPressed(KEYCODE_SPACE))
	{
		SceneManager::Get().SwitchScene(SceneManager::WORLD);
	}
	DLOG_CORE_INFO("MAIN MENU UPDATE");
}
