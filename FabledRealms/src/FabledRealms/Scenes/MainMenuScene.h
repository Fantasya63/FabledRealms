#pragma once
#include <Engine/Scene/Scene.h>

class MainMenuScene : public Scene
{
public:
	MainMenuScene();
	virtual void Update(const Time& const time) override;
};
