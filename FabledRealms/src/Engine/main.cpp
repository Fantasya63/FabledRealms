#include "frpch.h"

#include "Engine/Application.h"
#include "Engine/Window.h"
#include "Engine/Time.h"

extern Application* CreateApplication();

int main()
{
    Application* app = CreateApplication();
    app->Run();
    delete app;

    return 0;
}