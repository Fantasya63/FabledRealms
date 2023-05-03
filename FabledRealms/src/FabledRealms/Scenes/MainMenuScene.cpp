#include "frpch.h"
#include "Engine/Input/Input.h"

#include "Engine/Scene/SceneManager.h"
#include "MainMenuScene.h"
#include "glad/glad.h"

MainMenuScene::MainMenuScene()
{
	LOG_INFO("CREATED WORLD SCENE");
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
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
