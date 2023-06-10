#pragma once

#include "Engine/Application.h"

#include "Engine/Graphics/Shader.h"
#include "Engine/Graphics/Texture.h"
#include "Engine/Graphics/Camera.h"

#include "Engine/Time.h"

class FabledRealmsApp : public Application
{
public:
	FabledRealmsApp();
	~FabledRealmsApp();

	void OnUpdate(const Time& time) override;
};
