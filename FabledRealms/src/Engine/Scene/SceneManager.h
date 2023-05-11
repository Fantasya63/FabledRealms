#pragma once
#include "Scene.h"


class SceneManager
{
public:
	enum Scenes
	{
		MENU,
		WORLD,
	};

	static SceneManager& Get() { return *s_Instance; };
	SceneManager();
	~SceneManager();

	void UpdateScene(const Time& const time);
	void SwitchScene(Scenes scene);

private:
	static SceneManager* s_Instance;

	Scene* currentScene;
};

