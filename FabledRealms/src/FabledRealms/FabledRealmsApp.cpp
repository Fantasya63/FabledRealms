#include "frpch.h"
#include "FabledRealmsApp.h"
#include "Engine/Input/Input.h"
#include "Engine/Scene/SceneManager.h"

Application* CreateApplication()
{
	return new FabledRealmsApp();
}

FabledRealmsApp::FabledRealmsApp()
{
    LOG_INFO("          ");
    LOG_INFO("          ");
    LOG_INFO("           --------------- WELCOME! --------------");
    LOG_INFO("          ");
    LOG_INFO("          ");
    LOG_INFO("Controls: WASD for movement, Mouse to look");
    LOG_INFO("          Escape key to exit");
    LOG_INFO("          Hold Left Alt to enable mouse");
    LOG_INFO("          ");

    LOG_INFO("Building: Right click to Place Blocks");
    LOG_INFO("          Left  click to Remove Blocks");
    LOG_INFO("          X to cycle blocks forward");
    LOG_INFO("          Z to cycle blocks Backward");

    LOG_INFO("          1 - Select Bedrock");
    LOG_INFO("          2 - Select Stone");
    LOG_INFO("          3 - Select Dirt");
    LOG_INFO("          4 - Select Grass");
    LOG_INFO("          5 - Select Sand");
    LOG_INFO("          6 - Select Wood Plank");
    LOG_INFO("          7 - Select Wood Log");
    LOG_INFO("          8 - Select Brick");
    LOG_INFO("          9 - Select Leaves");
    LOG_INFO("          Note: There are more blocks left so cycle through them!");

    LOG_INFO("          ");
    LOG_INFO("          ");
    LOG_INFO("          ");
    LOG_INFO("          ");
    LOG_INFO("Credits : Title Menu Font: Minecrafter");
    LOG_INFO("          ");


    LOG_INFO("        : Calm and Peaceful by LesFM | https://lesfm.net/relaxing-background-music/");
    LOG_INFO("          Music promoted by https://www.chosic.com/free-music/all/");
    LOG_INFO("          Creative Commons CC BY 3.0");
    LOG_INFO("          https ://creativecommons.org/licenses/by/3.0/");
    LOG_INFO("          ");


    LOG_INFO("        : Vilja Pix 2.0 by ROllerozxa | https://content.minetest.net/packages/ROllerozxa/vilja_pix_2/");
    LOG_INFO("        : CC-BY-SA-4.0");
    LOG_INFO("        : https://spdx.org/licenses/CC-BY-SA-4.0.html");
    LOG_INFO("        : ");


    SceneManager::Get().SwitchScene(SceneManager::MENU);
}

FabledRealmsApp::~FabledRealmsApp()
{
	LOG_INFO("FABLED REALMS EXIT");
}

void FabledRealmsApp::OnUpdate(const Time& time)
{
    //Update The Scene
    SceneManager::Get().UpdateScene(time);
}
