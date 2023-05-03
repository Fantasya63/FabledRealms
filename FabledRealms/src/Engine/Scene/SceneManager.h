#pragma once
#include "Scene.h"


class SceneManager
{
public:
	enum Scenes
	{
		DEFAULT = 0, //Scene We'll be loading is the first index

		MENU = 0,
		WORLD,
	};

	static SceneManager& Get() { return *s_Instance; };
	SceneManager();

	void UpdateScene(const Time& const time);
	void SwitchScene(Scenes scene);

private:
	static SceneManager* s_Instance;

	Scene* currentScene;
};

