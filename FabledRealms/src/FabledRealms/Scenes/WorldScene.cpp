#include "frpch.h"

#include "glad/glad.h"
#include "Engine/Scene/SceneManager.h"
#include "WorldScene.h"
#include "Engine/Input/Input.h"
#include "Engine/Application.h"
#include "Engine/Sound/AudioManager.h"


//Global variables for this cpp file only
static const Camera* callbackCamera = nullptr;

static char currentBlock = VoxelData::BLOCK_ID::Stone;


void MouseCallback(int button, int action, int mods)
{
    FR_ASSERT(callbackCamera, "Callback camera is not set!");

    //Remove Blocks
    if (button == MOUSE_BUTTON_LEFT && action == KEY_PRESS)
    {
        Ray ray(callbackCamera->GetPosition(), callbackCamera->GetDirection());

        RayHit hit = World::Get().RayCast(ray);
        if (hit.IsVoxelHit)
        {
            //Remove the voxel ie change it to an Air block
            World::Get().ChangeVoxel(hit.VoxelPos, VoxelData::BLOCK_ID::Air);
        }
    }

    //Add block
    else if (button == MOUSE_BUTTON_RIGHT && action == KEY_PRESS)
    {
        Ray ray(callbackCamera->GetPosition(), callbackCamera->GetDirection());

        RayHit hit = World::Get().RayCast(ray);
        if (hit.IsVoxelHit)
        {
            //Add voxel at the hit position
            glm::ivec3 placePosition = hit.VoxelPos + hit.Normal;

            World::Get().ChangeVoxel(placePosition, currentBlock);
        }
    }
}

void KeyboardCallback(int button, int scancode, int action, int mods )
{
    //Swap the block to place if we pressed the corresponding buttons
    if (action == KEY_PRESS)
    {
        //Check what button is pressed
        switch (button)
        {
            //Next
            case KEYCODE_X:
                currentBlock += 1;
                if (currentBlock > VoxelData::TOTAL_NUM_OF_BLOCKS - 1)
                    currentBlock = 0;
                LOG_INFO("Current Block: " << (int)currentBlock);
                break;

            //Back
            case KEYCODE_Z:
                currentBlock -= 1;
                if (currentBlock < 0)
                    currentBlock = VoxelData::TOTAL_NUM_OF_BLOCKS - 1;
                LOG_INFO("Current Block: " << (int)currentBlock);
                break;

            //Bedrock
            case KEYCODE_1:
                currentBlock = VoxelData::BLOCK_ID::Bedrock;
                LOG_INFO("Block in hand: Bedrock");
                break;

            //Stone
            case KEYCODE_2:
                currentBlock = VoxelData::BLOCK_ID::Stone;
                LOG_INFO("Block in hand: Stone");
                break;

            //Dirt
            case KEYCODE_3:
                currentBlock = VoxelData::BLOCK_ID::Dirt;
                LOG_INFO("Block in hand: Dirt");
                break;

            //Grass
            case KEYCODE_4:
                currentBlock = VoxelData::BLOCK_ID::Grass;
                LOG_INFO("Block in hand: Grass");
                break;

            //Sand
            case KEYCODE_5:
                currentBlock = VoxelData::BLOCK_ID::Sand;
                LOG_INFO("Block in hand: Sand");
                break;

            //Wood Planks
            case KEYCODE_6:
                currentBlock = VoxelData::BLOCK_ID::WoodPlank;
                LOG_INFO("Block in hand: Wood Plank");
                break;

            //Wood Log
            case KEYCODE_7:
                currentBlock = VoxelData::BLOCK_ID::WoodLog;
                LOG_INFO("Block in hand: Wood Log");
                break;

            //Brick
            case KEYCODE_8:
                currentBlock = VoxelData::BLOCK_ID::Brick;
                LOG_INFO("Block in hand: Brick");
                break;

            //Leaves
            case KEYCODE_9:
                currentBlock = VoxelData::BLOCK_ID::Leaves;
                LOG_INFO("Block in hand: Leaves");
                break;
        }
    }
}

//---------------------------------------------------------------------------

WorldScene::WorldScene()
{

    DLOG_INFO("CREATED WORLD SCENE");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    //Set the mouse and keyboard callback function to the input system
    callbackCamera = &m_Camera;
    InputManager::Get().SetMouseButtonCallback(MouseCallback);
    InputManager::Get().SetKeyboardButtonCallback(KeyboardCallback);

    // ---------------------------------- Tone Mapping ----------------------------------

    m_TonemappingShader = new Shader("Assets/Shaders/TonemappingShader.vert", "Assets/Shaders/TonemappingShader.frag");

    m_TonemappingShader->Use();
    m_TonemappingShader->setInt("scene", 0);
    m_TonemappingShader->setInt("bloom", 1);





    //--------------------------------------- Frame Buffers -------------------------------------------------------



    Window* window = Application::Get().GetWindow();
    int width = window->GetWidth();
    int height = window->GetHeight();
  
    m_HDRBufffer = new HdrFBO();
    m_HDRBufffer->Init(width, height);

    m_BloomFBO = new BloomFBO();
    m_BloomFBO->Init(width, height, 5);

    m_GeometryBuffer = new GeomertryBuffer();
    m_GeometryBuffer->Init(width, height);
    
    Mesh::InitMeshFullScreenQuad(m_FullScreenQuadMesh);

    // ------------------------------------------------------- Crosshair ------------------------------------------------------
    
        
    //Create and Configure the shader
    m_CrosshairShader = new Shader("Assets/Shaders/CrosshairShader.vert", "Assets/Shaders/CrosshairShader.frag");
    m_CrosshairShader->setInt("CrosshairTex", 0);


    //Create the crosshair texture
    const char crosshairTexturePath[6][100] = {
        "Assets/Textures/crosshair.png",
    };
    m_CrosshairTexture = new Texture(crosshairTexturePath, Texture::TEXTURE_TYPE::TEXTURE2D, Texture::TEXTURE_FILTER::LINEAR);


    Mesh::InitMeshFullScreenQuad(m_CrosshairMesh);
    m_CrosshairMesh.DiffuseTexID = m_CrosshairTexture->GetRendererID();



    // ------------------------------------------------------- Cubemap --------------------------------------------------------
    
    //Create the cubemap texture
    const char cubmapTexturePath[6][100] = {
        "Assets/Textures/Cubemap/right.png",
        "Assets/Textures/Cubemap/left.png",
        "Assets/Textures/Cubemap/top.png",
        "Assets/Textures/Cubemap/bottom.png",
        "Assets/Textures/Cubemap/front.png",
        "Assets/Textures/Cubemap/back.png",
    };
    m_CubemapTexture = new Texture(cubmapTexturePath, Texture::TEXTURE_TYPE::CUBEMAP, Texture::TEXTURE_FILTER::LINEAR);


    Mesh::InitMeshCubemap(m_CubemapMesh);
    m_CubemapMesh.CubemapTexID = m_CubemapTexture->GetRendererID();

    //Create the shader for the sky
    m_CubemapShader = new Shader("Assets/Shaders/Cubemap.vert", "Assets/Shaders/Cubemap.frag");



    // ---------------------------------------------- World -----------------------------------------------------

    //Create and configure the shader for the terrain
    m_TerrainShader = new Shader("Assets/Shaders/TerrainShader.vert", "Assets/Shaders/TerrainShader.frag");
    m_TerrainShader->SetMat4("a_ProjMatrix", m_Camera.GetProjMatrix(Application::Get().GetWindow()->GetAspectRatio()));


   


    //Disable Mouse
    InputManager::SetMouseMode(InputManager::MouseMode::DISABLED);
}



void WorldScene::Update(const Time& const time)
{

    if (InputManager::IsKeyDown(KEYCODE_DELETE))
    {
        SceneManager::Get().SwitchScene(SceneManager::MENU);
        return;
    }

    if (InputManager::IsKeyDown(KEYCODE_LEFT_ALT))
    {
        InputManager::SetMouseMode(InputManager::MouseMode::NORMAL);
    }
    else
    {
        InputManager::SetMouseMode(InputManager::MouseMode::DISABLED);
    }

    


    m_Camera.Update(time);


    // ----------------------------- Rendering ------------------------------------------------

    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //Setup HDR FBO
    //m_HDRBufffer->Bind();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //Enable depth test, this is disabled when rendering the full screen quad
    //glEnable(GL_DEPTH_TEST); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    // ------------ Crosshair ------------
    // Bind the texture
    
    // Configure the shader
    m_CrosshairShader->Use();
    Window* window = Application::Get().GetWindow();
    glm::vec2 screenRes = glm::vec2(window->GetWidth(), window->GetHeight());
    m_CrosshairShader->SetVec2("u_ScreenRes", screenRes);


    m_CrosshairMesh.RenderMesh(*m_CrosshairShader);




    // ----------- World -------------------

    // Configure the shader
    //m_TerrainShader->Use();
    //glm::mat4 view = m_Camera.GetViewMatrix();
    //m_TerrainShader->SetMat4("a_ViewMatrix", m_Camera.GetViewMatrix());
    //m_TerrainShader->SetMat4("a_ProjMatrix", m_Camera.GetProjMatrix(Application::Get().GetWindow()->GetAspectRatio()));
    //m_TerrainShader->setInt("blockTex", 0);
    //m_TerrainShader->SetFloat("u_Time", time.currentTime);
    //
    ////Render the world
    //m_World.Render(m_TerrainShader);



    // ----- Render Skybox -------
    // change depth function so depth test passes when values are equal to depth buffer's content
    glDepthFunc(GL_EQUAL);  
    
    
    //Configure the shader
    m_CubemapShader->Use();
    m_CubemapShader->SetMat4("a_ViewMatrix", glm::mat4(glm::mat3(m_Camera.GetViewMatrix()))); // Strip away the translations in the matrix
    m_TerrainShader->SetMat4("a_ProjMatrix", m_Camera.GetProjMatrix(Application::Get().GetWindow()->GetAspectRatio()));

    //Render the Geometry
    m_CubemapMesh.RenderMesh(*m_CubemapShader);

    // Change Depth func back to default
    //glDepthFunc(GL_LESS); 

   // m_HDRBufffer->UnBind();




    // -----------------------------------------------------------------------------------
    //glDisable(GL_DEPTH_TEST);

    //glClear(GL_COLOR_BUFFER_BIT);
    //m_BloomFBO->RenderBloomTexture(m_FullScreenQuadMesh, m_HDRBufffer->GetColorAttachmentID(0), 0.004f);

    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //glDisable(GL_BLEND);
    //Render to screen with tonemapping
    //glClear(GL_COLOR_BUFFER_BIT);
   
    //m_TonemappingShader->Use();
    //m_TonemappingShader->setInt("scene", 0);
    //m_TonemappingShader->setInt("bloom", 1);

   // m_FullScreenQuadMesh.ActivateTexture(GL_TEXTURE0, m_HDRBufffer->GetColorAttachmentID(0));
    //m_FullScreenQuadMesh.ActivateTexture(GL_TEXTURE1, m_BloomFBO->GetBloomTexture());
    
    //m_TonemappingShader->setInt("scene", 0);
    //m_TonemappingShader->setInt("bloom", 1);
    //m_FullScreenQuadMesh.RenderMesh();

    if (InputManager::IsKeyDown(KEYCODE_ESCAPE))
        Application::Get().RequestClose();
}

WorldScene::~WorldScene()
{
    delete m_HDRBufffer;
    delete m_BloomFBO;
    delete m_GeometryBuffer;

    delete m_CubemapShader;
    delete m_CubemapTexture;

    delete m_TerrainShader;

    delete m_CrosshairShader;
    delete m_CrosshairTexture;

    delete m_TonemappingShader;

    Mesh::CleanUpMesh(m_CubemapMesh);
    Mesh::CleanUpMesh(m_FullScreenQuadMesh);
    Mesh::CleanUpMesh(m_CrosshairMesh);
}

void WorldScene::OnWindowResized(int width, int height)
{
    DLOG_CORE_INFO("Window Resized: " << width << ", " << height);

    m_HDRBufffer->UnBind();

    //Delete Frame Buffers
    delete m_HDRBufffer;
    delete m_BloomFBO;
    delete m_GeometryBuffer;

    m_HDRBufffer = new HdrFBO();
    m_HDRBufffer->Init(width, height);

    m_BloomFBO = new BloomFBO();
    m_BloomFBO->Init(width, height, 5);

    m_GeometryBuffer = new GeomertryBuffer();
    m_GeometryBuffer->Init(width, height);

    //m_BloomFBO_Old.Resize(width, height);
}