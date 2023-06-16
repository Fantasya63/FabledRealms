#include "frpch.h"

#include "Engine/Application.h"
#include "Engine/Input/Input.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Sound/AudioManager.h"

#include "MainMenuScene.h"
#include "glad/glad.h"

MainMenuScene::MainMenuScene()
{
	//Set OpenGl's Winding order
	glFrontFace(GL_CCW);

	//Enable Culling
    glEnable(GL_CULL_FACE);

    //Cull backfaces to save GPU resources
    glCullFace(GL_BACK);



	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);


	//Create the main menu shader
	m_MenuShader = new Shader("Assets/Shaders/MainMenuShader.vert", "Assets/Shaders/MainMenuShader.frag");

	//Create the Main Menu texture
	const std::string path = "Assets/Textures/FabledRealmsSplash.jpg";
	m_MenuTexture.InitTexture2D(path, Texture::TEXTURE_FILTER::LINEAR, true, true); //Its not possible to not have mipmaps in opengl? or generally?

	//Init mesh
	Mesh::InitMeshFullScreenQuad(m_ScreenQuadMesh);
	m_ScreenQuadMesh.DiffuseTexID = m_MenuTexture.GetRendererID();



	//Stop all background music if there is one playing already
	AudioManager::Get().StopAllSounds();

	//Play background music
	AudioManager::Get().PlaySound("Assets/Audio/Menu/Calm-and-Peaceful.mp3", true);

	//Enable Mouse
	InputManager::SetMouseMode(InputManager::MouseMode::NORMAL);

	DLOG_INFO("CREATED WORLD SCENE");
}

MainMenuScene::~MainMenuScene()
{
	//AudioManager::Get().StopAllSounds();

	delete m_MenuShader;
	
	Mesh::CleanUpMesh(m_ScreenQuadMesh);
}

void MainMenuScene::Update(const Time& const time)
{
	//Clear the frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	//Close if we pressed escape
	if (InputManager::IsKeyDown(KEYCODE_ESCAPE))
	{
		Application::Get().RequestClose();
		return;
	}

	//Swith to the world scene if we pressed enter
	else if (InputManager::IsKeyDown(KEYCODE_SPACE))
	{
		SceneManager::Get().SwitchScene(SceneManager::WORLD);
		return;
	}

	// -- RENDERING ------------------------------------
	m_MenuShader->Use();
	Window* window = Application::Get().GetWindow();
	glm::vec2 screenRes = glm::vec2(window->GetWidth(), window->GetHeight());
	m_MenuShader->SetVec2("u_ScreenRes", screenRes);
	m_MenuShader->SetFloat("u_Time", time.currentTime);

	m_ScreenQuadMesh.RenderMesh(*m_MenuShader);
}
