#include "frpch.h"
#include "FabledRealmsApp.h"
#include "glad/glad.h"


Application* CreateApplication()
{
	return new FabledRealmsApp();
}

FabledRealmsApp::FabledRealmsApp()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    m_Shader = new Shader("Assets/Shaders/testShader.vert", "Assets/Shaders/testShader.frag");
    
    float* vertices = new float[12]{
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    uint32_t* indices = new uint32_t[6]{  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    m_HelloTriangleVAO = VertexArray::Create();
    m_HelloTriangleVAO->Bind();
   
    m_HelloTriangleVBO = VertexBuffer::Create(vertices, sizeof(float) * 12);
    m_HelloTriangleIBO = IndexBuffer::Create(indices, 6);
    
    //Set Vertex Layouts
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

FabledRealmsApp::~FabledRealmsApp()
{
    delete m_HelloTriangleVAO;
    delete m_HelloTriangleVBO;
    delete m_HelloTriangleIBO;
    delete m_Shader;

	LOG_INFO("FABLED REALMS EXIT");
}

void FabledRealmsApp::OnUpdate(const Time& time)
{
    glClear(GL_COLOR_BUFFER_BIT);
  
    //LOG_INFO("Time: " << time.deltaTime);

    m_Shader->Use();
    m_Shader->SetFloat("u_Time", time.currentTime);

    m_HelloTriangleVAO->Bind();
    glDrawElements(GL_TRIANGLES, m_HelloTriangleIBO->GetCount(), GL_UNSIGNED_INT, 0);
}
