#include "frpch.h"

#include "glad/glad.h"
#include "Engine/Scene/SceneManager.h"
#include "WorldScene.h"
#include "Engine/Input/Input.h"
#include "Engine/Application.h"

WorldScene::WorldScene()
{
    LOG_INFO("CREATED WORLD SCENE");
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

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
        0, 3, 2,
        0, 2, 1,

        //Front Face
        4, 5, 6,
        4, 6, 7,

        //Left
        1, 2, 6,
        1, 6, 5,

        //Right
        4, 7, 3,
        4, 3, 0,

        //Top
        4, 0, 1,
        4, 1, 5,

        //Bottom
        7, 6, 2,
        7, 2, 3
    };

    m_CubemapVAO = VertexArray::Create();
    m_CubemapVAO->Bind();

    m_CubemapVBO = VertexBuffer::Create(skyboxVertices, sizeof(float) * 64); //Takes in the size of vertices in bytes
  
    int indexCount = sizeof(skyboxIndices) / sizeof(skyboxIndices[0]);
    m_CubemapIBO = IndexBuffer::Create(skyboxIndices, indexCount);


    m_CubemapShader = new Shader("Assets/Shaders/Cubemap.vert", "Assets/Shaders/Cubemap.frag");



    // ---------------------------------------------- World -----------------------------------------------------

	

    m_Shader = new Shader("Assets/Shaders/testShader.vert", "Assets/Shaders/testShader.frag");

    const char texturePath[6][100] = {
        "Assets/Textures/grass_block_side.png",
    };

    m_Texture = new Texture(texturePath, Texture::TEXTURE_TYPE::TEXTURE2D, Texture::TEXTURE_FILTER::NEAREST);

    float vertices[] {
        //POS              NORMAL          UV
        0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,     // top right
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // bottom right
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,     // bottom left
       -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,     // top left 
    };

    uint32_t indices[] {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    m_HelloTriangleVAO = VertexArray::Create();
    m_HelloTriangleVAO->Bind();

    m_HelloTriangleVBO = VertexBuffer::Create(vertices, sizeof(float) * 32);
    m_HelloTriangleIBO = IndexBuffer::Create(indices, 6);

    // TODO: Update the Proj Martix when we resize or change window size
    m_Shader->SetMat4("a_ProjMatrix", m_Camera.GetProjMatrix(Application::Get().GetWindow()->GetAspectRatio()));
}



void WorldScene::Update(const Time& const time)
{
    LOG_INFO("WORLD UPDATE");
    if (Input::IsKeyPressed(KEYCODE_DELETE))
    {
        SceneManager::Get().SwitchScene(SceneManager::MENU);
        return;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    m_Camera.Update(time);


    // ----------------------------- Rendering ------------------------------------------------


    m_Texture->Bind();
    m_Shader->Use();

    glm::mat4 view = m_Camera.GetViewMatrix();
    glm::vec3 test = view * glm::vec4(0.0, 0.0, 0.0, 1.0);
    //DLOG_CORE_INFO("VIEW TEST: " << test.x << ", " << test.y << ", " << test.z);

    m_Shader->SetMat4("a_ViewMatrix", m_Camera.GetViewMatrix());
    m_Shader->SetMat4("a_ProjMatrix", m_Camera.GetProjMatrix(Application::Get().GetWindow()->GetAspectRatio()));
    m_Shader->setInt("blockTex", 0);
    m_Shader->SetFloat("u_Time", time.currentTime);

    m_HelloTriangleVAO->Bind();
    glDrawElements(GL_TRIANGLES, m_HelloTriangleIBO->GetCount(), GL_UNSIGNED_INT, 0);


    // ----- Render Skybox -------

    glDepthFunc(GL_EQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    
    m_CubemapTexture->Bind();
    m_CubemapShader->Use();
    m_CubemapShader->SetMat4("a_ViewMatrix", glm::mat4(glm::mat3(m_Camera.GetViewMatrix()))); // Strip away the translations in the matrix
    m_Shader->SetMat4("a_ProjMatrix", m_Camera.GetProjMatrix(Application::Get().GetWindow()->GetAspectRatio()));

    m_CubemapVAO->Bind();
    glDrawElements(GL_TRIANGLES, m_CubemapIBO->GetCount(), GL_UNSIGNED_INT, 0);

    glDepthFunc(GL_LESS); // Change Depth func back to default

    // -----------------------------------------------------------------------------------

    if (Input::IsKeyPressed(KEYCODE_ESCAPE))
        Application::Get().RequestClose();
}

WorldScene::~WorldScene()
{
    delete m_CubemapVAO;
    delete m_CubemapVBO;
    delete m_CubemapIBO;
    delete m_CubemapShader;
    delete m_CubemapTexture;

    delete m_HelloTriangleVAO;
    delete m_HelloTriangleVBO;
    delete m_HelloTriangleIBO;
    delete m_Shader;
    delete m_Texture;

	LOG_INFO("DELETED WORLD SCENE");
}
