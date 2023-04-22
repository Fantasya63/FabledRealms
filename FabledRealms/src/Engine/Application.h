#pragma once
class Application
{
public:
	virtual ~Application() {};

	virtual void OnUpdate(float deltaTime) {};

private:
	bool m_Running = true;
};