#include "frpch.h"

#include "glad/glad.h"
#include "Engine/Scene/SceneManager.h"
#include "WorldScene.h"
#include "Engine/Input/Input.h"
#include "Engine/Application.h"

//Global variables for this cpp file only
static const Camera* callbackCamera = nullptr;

static char currentBlock = Chunk::BLOCK_ID::Stone;

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
            World::Get().ChangeVoxel(hit.VoxelPos, Chunk::BLOCK_ID::Air);
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
        switch (button)
        {
            //Grass
            case KEYCODE_1:
                currentBlock = Chunk::BLOCK_ID::Grass;
                LOG_INFO("Block in hand: Grass");
                break;

            //Dirt
            case KEYCODE_2:
                currentBlock = Chunk::BLOCK_ID::Dirt;
                LOG_INFO("Block in hand: Dirt");
                break;

            //Stone
            case KEYCODE_3:
                currentBlock = Chunk::BLOCK_ID::Stone;
                LOG_INFO("Block in hand: Stone");
                break;

            //Bedrock
            case KEYCODE_4:
                currentBlock = Chunk::BLOCK_ID::Bedrock;
                LOG_INFO("Block in hand: Bedrock");
                break;
        }
    }
}

//---------------------------------------------------------------------------

WorldScene::WorldScene()
{

    LOG_INFO("CREATED WORLD SCENE");
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    //Set the mouse and keyboard callback function to the input system
    callbackCamera = &m_Camera;
    InputManager::Get().SetMouseButtonCallback(MouseCallback);
    InputManager::Get().SetKeyboardButtonCallback(KeyboardCallback);

    // ------------------------------------------------------- Cubemap --------------------------------------------------------
    
    const char cubmapTexturePath[6][100] = {
        "Assets/Textures/Cubemap/right.png",
        "Assets/Textures/Cubemap/left.png",
        "Assets/Textures/Cubemap/top.png",
        "Assets/Textures/Cubemap/bottom.png",
        "Assets/Textures/Cubemap/front.png",
        "Assets/Textures/Cubemap/back.png",
    };
    m_CubemapTexture = new Texture(cubmapTexturePath, Texture::TEXTURE_TYPE::CUBEMAP, Texture::TEXTURE_FILTER::LINEAR);

    //Interleaved Vertex Data for Skybox
    float skyboxVertices[] = {
       
        //Positions          Normal            UV
        -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Back Top Left   [0]
         1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Back Top Right  [1]
         1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Back Bot Right  [2]
        -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Back Bot Left   [3]

        -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Front Top Left  [4]
         1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Front Top Right [5]
         1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Front Bot Right [6]
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Front Bot Left  [7]
    };

    // Index of the vertices for the skybox
    // Refer to skyboxVertices Indices
    uint32_t skyboxIndices[] =
    {
        //Back Face
        0, 2, 3,
        0, 1, 2,

        //Front Face
        4, 7, 6,
        4, 6, 5,

        //Left
        4, 3, 7,
        4, 0, 3,

        //Right
        1, 6, 2,
        1, 5, 6,

        //Top
        4, 1, 0,
        4, 5, 1,

        //Bottom
        7, 3, 2,
        7, 2, 6
    };

    m_CubemapVAO = VertexArray::Create();
    m_CubemapVAO->Bind();

    m_CubemapVBO = VertexBuffer::Create(skyboxVertices, sizeof(float) * 64); //Takes in the size of vertices in bytes
  
    int indexCount = sizeof(skyboxIndices) / sizeof(skyboxIndices[0]);
    m_CubemapIBO = IndexBuffer::Create(skyboxIndices, indexCount);


    m_CubemapShader = new Shader("Assets/Shaders/Cubemap.vert", "Assets/Shaders/Cubemap.frag");



    // ---------------------------------------------- World -----------------------------------------------------

	

    m_TerrainShader = new Shader("Assets/Shaders/TerrainShader.vert", "Assets/Shaders/TerrainShader.frag");

    const char texturePath[6][100] = {
        "Assets/Textures/blocks.jpg",
    };

    m_TerrainTexture = new Texture(texturePath, Texture::TEXTURE_TYPE::TEXTURE2D, Texture::TEXTURE_FILTER::NEAREST);


    // TODO: Update the Proj Martix when we resize or change window size
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    m_Camera.Update(time);


    // ----------------------------- Rendering ------------------------------------------------


    m_TerrainTexture->Bind();
    m_TerrainShader->Use();

    glm::mat4 view = m_Camera.GetViewMatrix();
    //DLOG_CORE_INFO("VIEW TEST: " << test.x << ", " << test.y << ", " << test.z);

    m_TerrainShader->SetMat4("a_ViewMatrix", m_Camera.GetViewMatrix());
    m_TerrainShader->SetMat4("a_ProjMatrix", m_Camera.GetProjMatrix(Application::Get().GetWindow()->GetAspectRatio()));
    m_TerrainShader->setInt("blockTex", 0);
    m_TerrainShader->SetFloat("u_Time", time.currentTime);


    m_World.Render(m_TerrainShader);

    // ----- Render Skybox -------

    glDepthFunc(GL_EQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    
    m_CubemapTexture->Bind();
    m_CubemapShader->Use();
    m_CubemapShader->SetMat4("a_ViewMatrix", glm::mat4(glm::mat3(m_Camera.GetViewMatrix()))); // Strip away the translations in the matrix
    m_TerrainShader->SetMat4("a_ProjMatrix", m_Camera.GetProjMatrix(Application::Get().GetWindow()->GetAspectRatio()));

    m_CubemapVAO->Bind();
    glDrawElements(GL_TRIANGLES, m_CubemapIBO->GetCount(), GL_UNSIGNED_INT, 0);

    glDepthFunc(GL_LESS); // Change Depth func back to default

    // -----------------------------------------------------------------------------------

    if (InputManager::IsKeyDown(KEYCODE_ESCAPE))
        Application::Get().RequestClose();
}

WorldScene::~WorldScene()
{
    delete m_CubemapVAO;
    delete m_CubemapVBO;
    delete m_CubemapIBO;
    delete m_CubemapShader;
    delete m_CubemapTexture;

    delete m_TerrainShader;
    delete m_TerrainTexture;

	LOG_INFO("DELETED WORLD SCENE");
}
