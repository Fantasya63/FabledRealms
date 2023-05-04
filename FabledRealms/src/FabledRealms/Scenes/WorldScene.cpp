#include "frpch.h"

#include "glad/glad.h"
#include "Engine/Scene/SceneManager.h"
#include "WorldScene.h"
#include "Engine/Input/Input.h"
#include "Engine/Application.h"

WorldScene::WorldScene()
{
	LOG_INFO("CREATED WORLD SCENE");
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    m_Shader = new Shader("Assets/Shaders/testShader.vert", "Assets/Shaders/testShader.frag");
    m_Texture = new Texture("Assets/Textures/grass_block_side.png");

    float* vertices = new float[20] {
        // POS                  UV
        0.5f,  0.5f, 0.0f, 1.0f, 1.0f,     // top right
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,     // bottom right
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,     // bottom left
       -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,     // top left 
    };

    uint32_t* indices = new uint32_t[6]{  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    m_HelloTriangleVAO = VertexArray::Create();
    m_HelloTriangleVAO->Bind();

    m_HelloTriangleVBO = VertexBuffer::Create(vertices, sizeof(float) * 20);
    m_HelloTriangleIBO = IndexBuffer::Create(indices, 6);

    //Set Vertex Layouts
    // 
    //POSITION
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //UV
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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

    //Renderering
    m_Texture->Bind();
    m_Shader->Use();

    glm::mat4 view = m_Camera.GetViewMatrix();
    glm::vec3 test = view * glm::vec4(0.0, 0.0, 0.0, 1.0);
    //DLOG_CORE_INFO("VIEW TEST: " << test.x << ", " << test.y << ", " << test.z);

    m_Shader->SetMat4("a_ViewMatrix", m_Camera.GetViewMatrix());
    m_Shader->SetMat4("a_ProjMatrix", m_Camera.GetProjMatrix(Application::Get().GetWindow()->GetAspectRatio())); // TODO: Only set the projection matrix when it changes
    m_Shader->setInt("blockTex", 0);
    m_Shader->SetFloat("u_Time", time.currentTime);

    m_HelloTriangleVAO->Bind();
    glDrawElements(GL_TRIANGLES, m_HelloTriangleIBO->GetCount(), GL_UNSIGNED_INT, 0);

    if (Input::IsKeyPressed(KEYCODE_ESCAPE))
        Application::Get().RequestClose();
}

WorldScene::~WorldScene()
{
    delete m_HelloTriangleVAO;
    delete m_HelloTriangleVBO;
    delete m_HelloTriangleIBO;
    delete m_Shader;
    delete m_Texture;

	LOG_INFO("DELETED WORLD SCENE");
}
