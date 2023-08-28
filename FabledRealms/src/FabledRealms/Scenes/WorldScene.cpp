#include "frpch.h"

#include "glad/glad.h"
#include "Engine/Scene/SceneManager.h"
#include "WorldScene.h"
#include "Engine/Input/Input.h"
#include "Engine/Application.h"
#include "Engine/Sound/AudioManager.h"
#include "Engine/Graphics/TextureAtlass.h"

//Global variables for this cpp file only
static const Camera* callbackCamera = nullptr;

static char currentBlock = VoxelData::BLOCK_ID::Stone;


std::vector<glm::mat4> getLightSpaceMatrices(Camera& cam, float aspect, const glm::vec3& const lightDir);

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

void KeyboardCallback(int button, int scancode, int action, int mods)
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

std::vector<float> shadowCascadeLevels;

std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view)
{
    const auto inv = glm::inverse(proj * view);

    std::vector<glm::vec4> frustumCorners;
    for (unsigned int x = 0; x < 2; ++x)
    {
        for (unsigned int y = 0; y < 2; ++y)
        {
            for (unsigned int z = 0; z < 2; ++z)
            {
                const glm::vec4 pt =
                    inv * glm::vec4(
                        2.0f * x - 1.0f,
                        2.0f * y - 1.0f,
                        2.0f * z - 1.0f,
                        1.0f);
                frustumCorners.push_back(pt / pt.w);
            }
        }
    }

    return frustumCorners;
}


//Rewrite all of this to use view space coords?
//Will probably help in the shadow disapearing artifact near the camera
glm::mat4 GetLightViewProjMatrix(Camera& cam, float near, float far, float fov, float aspectRatio, const glm::vec3& LightDir)
{
    const auto proj = glm::perspective(glm::radians(fov), aspectRatio, near, far);

    std::vector<glm::vec4> frustomCorners = getFrustumCornersWorldSpace(proj, cam.GetViewMatrix());

    glm::vec3 center = glm::vec3(0, 0, 0);
    for (const auto& v : frustomCorners)
    {
        center += glm::vec3(v);
    }
    center /= frustomCorners.size();

    const auto lightView = glm::lookAt(
        center + LightDir,
        center,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::lowest();
    for (const auto& v : frustomCorners)
    {
        const auto trf = lightView * v;
        minX = std::min(minX, trf.x);
        maxX = std::max(maxX, trf.x);
        minY = std::min(minY, trf.y);
        maxY = std::max(maxY, trf.y);
        minZ = std::min(minZ, trf.z);
        maxZ = std::max(maxZ, trf.z);
    }

    // Tune this parameter according to the scene
    constexpr float zMult = 32.0f;
    if (minZ < 0)
    {
        minZ *= zMult;
    }
    else
    {
        minZ /= zMult;
    }
    if (maxZ < 0)
    {
        maxZ /= zMult;
    }
    else
    {
        maxZ *= zMult;
    }

    const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

    return lightProjection * lightView;
}


std::vector<glm::mat4> getLightSpaceMatrices(Camera& cam, float aspect, const glm::vec3& const lightDir)
{
    std::vector<glm::mat4> ret;

    glm::vec2 camClipPlanes = cam.GetCamNearFarPlanes();

    const int numCascades = shadowCascadeLevels.size();
    float fov = cam.GetFOV();

    for (size_t i = 0; i < numCascades + 1; ++i)
    {
        if (i == 0)
        {
            ret.push_back(GetLightViewProjMatrix(cam, camClipPlanes.x, shadowCascadeLevels[i], fov, aspect, lightDir));
        }
        else if (i < numCascades)
        {
            ret.push_back(GetLightViewProjMatrix(cam, shadowCascadeLevels[i - 1], shadowCascadeLevels[i], fov, aspect, lightDir));
        }
        else
        {
            ret.push_back(GetLightViewProjMatrix(cam, shadowCascadeLevels[i - 1], camClipPlanes.y, fov, aspect, lightDir));
        }
    }
    return ret;
}







float waterLevel = 54.70f;







WorldScene::WorldScene()
{

    const glm::vec2 camClipPlanes = m_Camera.GetCamNearFarPlanes();
    shadowCascadeLevels = { camClipPlanes.y / 64, camClipPlanes.y / 24.0f,  camClipPlanes.y / 8.0f,  camClipPlanes.y / 2.0f };
    shadowCascadeLevels = { 8.0f, 16.0f,  32.0f, 64.0f };

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
    m_TonemappingShader->SetInt("scene", 0);
    m_TonemappingShader->SetInt("bloom", 1);





    //--------------------------------------- Frame Buffers -------------------------------------------------------



    Window* window = Application::Get().GetWindow();
    int width = window->GetWidth();
    int height = window->GetHeight();

    m_HDRBufffer = new HdrFBO();
    m_HDRBufffer->Init(width, height);

    m_BloomFBO = new BloomFBO();
    m_BloomFBO->Init(width, height, 5);

    m_GeometryBuffer = new GeometryBuffer();
    m_GeometryBuffer->Init(width, height);

    m_ShadowFBO = new ShadowFBO();
    m_ShadowFBO->Init(1024, 1024, shadowCascadeLevels.size());
    
    m_SSRFBO = new ScreenSpaceReflectionFBO();
    m_SSRFBO->Init(width, height);

    m_GeometryBufferShader = new Shader("Assets/Shaders/GeometryBufferShader.vert",
        "Assets/Shaders/GeometryBufferShader.frag");

    m_DefferedLightingShader = new Shader("Assets/Shaders/DefferedLightingShader.vert",
        "Assets/Shaders/DefferedLightingShader.frag");

    m_DefferedLightingShader->Use();
    m_DefferedLightingShader->SetInt("PositionEmissionTex", 0);
    m_DefferedLightingShader->SetInt("ColorMetallicTex", 1);
    m_DefferedLightingShader->SetInt("NormalRoughnessTex", 2);



    // ------------------------------------------------------- Crosshair ------------------------------------------------------


    //Create and Configure the shader
    m_CrosshairShader = new Shader("Assets/Shaders/CrosshairShader.vert", "Assets/Shaders/CrosshairShader.frag");
    m_CrosshairShader->SetInt("CrosshairTex", 0);


    //Create the crosshair texture
    const std::string crosshairTexturePath = "Assets/Textures/crosshair.png";
    m_CrosshairTexture.InitTexture2D(crosshairTexturePath, Texture::TEXTURE_FILTER::LINEAR, true, true, false);

    Mesh::InitMeshFullScreenQuad(m_CrosshairMesh);
    m_CrosshairMesh.DiffuseTexID = m_CrosshairTexture.GetRendererID();


    m_ShadowMapShader = new Shader("Assets/Shaders/Core/ShadowMap.vert", "Assets/Shaders/Core/ShadowMap.frag", "Assets/Shaders/Core/ShadowMap.geo");

    // ------------------------------------------------------- Cubemap --------------------------------------------------------

    const std::string cubemapPaths[] = {
        "Assets/Textures/Cubemap/right.png",
        "Assets/Textures/Cubemap/left.png",
        "Assets/Textures/Cubemap/top.png",
        "Assets/Textures/Cubemap/bottom.png",
        "Assets/Textures/Cubemap/front.png",
        "Assets/Textures/Cubemap/back.png",
    };

    m_CubemapTexture.InitEquirectangularMap("Assets/Environment/kloppenheim_06_puresky_4k.hdr", m_DiffuseIrradianceTexture, m_prefilteredTexture, m_brdfTexture);
    //m_CubemapTexture.InitEquirectangularMap("Assets/Environment/ninomaru_teien_4k.hdr", m_DiffuseIrradianceTexture, m_prefilteredTexture, m_brdfTexture);
    //m_CubemapTexture.InitCubemapTexture(cubemapPaths);

    m_BakedBRDFTexture.InitTexture2D("Assets/Textures/ibl_brdf_lut.png", Texture::TEXTURE_FILTER::LINEAR, false, false);


    Mesh::InitMeshCubemap(m_CubemapMesh);
    m_CubemapMesh.CubemapTexID = m_CubemapTexture.GetRendererID();
    //m_CubemapMesh.CubemapTexID =.GetRendererID();

    //Create the shader for the sky
    m_CubemapShader = new Shader("Assets/Shaders/Cubemap.vert", "Assets/Shaders/Cubemap.frag");


    Mesh::InitMeshWaterPlane(m_WaterPlaneMesh);
    m_WaterPlaneShader = new Shader("Assets/Shaders/WaterPlane.vert", "Assets/Shaders/WaterPlane.frag");
    
    m_WaterNormalTexture.InitTexture2D("Assets/Textures/waterNormal.png", Texture::TEXTURE_FILTER::LINEAR, false, false, true);

    //Disable Mouse
    InputManager::SetMouseMode(InputManager::MouseMode::DISABLED);
}

const glm::vec3 lightDir = glm::normalize(glm::vec3(1.0, 0.25, 0.75));

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



    Window* window = Application::Get().GetWindow();
    glm::vec2 screenRes = glm::vec2(window->GetWidth(), window->GetHeight());
    float aspect = screenRes.x / screenRes.y;


    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 0.0, 0.0, 0.0);
    glDepthFunc(GL_LEQUAL);


    // Shader Common Uniforms

    glm::mat4 view = m_Camera.GetViewMatrix();
    glm::mat4 invView = glm::inverse(view);
    glm::mat4 projMatrix = m_Camera.GetProjMatrix(Application::Get().GetWindow()->GetAspectRatio());
    glm::mat4 invProjMatrix = glm::inverse(projMatrix);
    glm::mat4 viewProj = projMatrix * view;







    //glViewport(0, 0, 1024, 1024);
    glm::ivec2 shadowRes = m_ShadowFBO->GetResolution();



    //Setup UBO
    const auto lightMatrices = getLightSpaceMatrices(m_Camera, aspect, lightDir);
    m_ShadowFBO->BindUniformUBO();
    for (int i = 0; i < lightMatrices.size(); ++i)
    {
        glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(glm::mat4x4), sizeof(glm::mat4x4), &lightMatrices[i]);
    }
    m_ShadowFBO->UnBindUniformUBO();


    m_ShadowMapShader->Use();
    m_ShadowFBO->Bind();
    glViewport(0, 0, shadowRes.x, shadowRes.y);
    glClear(GL_DEPTH_BUFFER_BIT);

    glCullFace(GL_FRONT);
    m_World.Render(m_ShadowMapShader, glm::mat4(1.0f));
    glCullFace(GL_BACK);

   // glBindFramebuffer(GL_FRAMEBUFFER, 0);






    //// ----------------------------- Rendering ------------------------------------------------
    //
    ////glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    ////Setup HDR FBO
    m_GeometryBuffer->Bind();
    glViewport(0, 0, screenRes.x, screenRes.y);

    unsigned int attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);



    // ----------- World -------------------

    // Configure the shader
    //m_TerrainShader->Use();
    m_GeometryBufferShader->Use();

    m_GeometryBufferShader->SetMat4("a_ViewMatrix", view);
    m_GeometryBufferShader->SetMat4("a_ProjMatrix", projMatrix);
    m_GeometryBufferShader->SetFloat("u_Time", time.currentTime);

    //Render the world
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_World.Render(m_GeometryBufferShader, view);


    //Deffered Lighting

    m_HDRBufffer->Bind();
    glDisable(GL_DEPTH_TEST);

    m_DefferedLightingShader->Use();
    m_DefferedLightingShader->SetInt("PositionEmissionTex", 0);
    m_DefferedLightingShader->SetInt("ColorMetallicTex", 1);
    m_DefferedLightingShader->SetInt("NormalRoughnessTex", 2);
    m_DefferedLightingShader->SetInt("irradianceMap", 5);
    m_DefferedLightingShader->SetInt("prefilteredMap", 6);
    m_DefferedLightingShader->SetInt("brdfLUT", 7);
    m_DefferedLightingShader->SetInt("shadowMap", 8);
    m_DefferedLightingShader->SetMat4("view", view);
    m_DefferedLightingShader->SetMat4("u_InvViewMatrix", invView);

    m_DefferedLightingShader->SetInt("cascadeCount", shadowCascadeLevels.size());
    m_DefferedLightingShader->SetFloat("farPlane", m_Camera.GetCamNearFarPlanes().y);
    for (int i = 0; i < shadowCascadeLevels.size(); i++)
    {
        m_DefferedLightingShader->SetFloat(("cascadePlaneDistances[" + std::to_string(i) + "]").c_str(), shadowCascadeLevels[i]);
    }


    m_DefferedLightingShader->SetVec3("LightDir", lightDir);

    glBindVertexArray(m_CrosshairMesh.VAO);



    //Setup texture Units

    //PositionEmission;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_GeometryBuffer->GetColorAttachmentID(0));


    //ColorMetallic;
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_GeometryBuffer->GetColorAttachmentID(1));

    //NormalRoughness;
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_GeometryBuffer->GetColorAttachmentID(2));


    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_DiffuseIrradianceTexture.GetRendererID());


    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilteredTexture.GetRendererID());


    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, m_brdfTexture.GetRendererID());


    glActiveTexture(GL_TEXTURE8);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_ShadowFBO->GetDepthAttachmentID());

    //Lighting Pass
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    glActiveTexture(GL_TEXTURE0);

   





    //Water
    //glm::mat4 waterModelMatrix = view;
    //waterModelMatrix = glm::scale(waterModelMatrix, glm::vec3(10.0f, 1.0f, 10.0f));
    //waterModelMatrix = glm::translate(waterModelMatrix, glm::vec3(0.0f, waterLevel, 0.0f));
    //
    //glm::mat3 viewNormalMatrix = glm::transpose(glm::inverse(view));
    //
    //
    //m_WaterPlaneShader->Use();
    //m_WaterPlaneShader->SetMat4("u_ModelViewMatrix", waterModelMatrix);
    //m_WaterPlaneShader->SetMat4("u_ViewMatrix", view);
    //m_WaterPlaneShader->SetMat4("u_ProjMatrix", projMatrix);
    //m_WaterPlaneShader->SetMat4("u_InvProjMatrix", invProjMatrix);
    //m_WaterPlaneShader->SetMat3("u_ViewNormalMatrix", viewNormalMatrix);
    //m_WaterPlaneShader->SetMat4("u_InvViewMatrix", invView);
    //
    //m_WaterPlaneShader->SetVec3("u_LightDir", viewNormalMatrix * lightDir);
    //m_WaterPlaneShader->SetMat3("u_ViewWorldNormalMatrix", glm::transpose(glm::inverse(invView)));
    //
    //m_WaterPlaneShader->SetFloat("u_WaterWorldLevel", waterLevel);
    //
    //
    //m_WaterPlaneShader->SetInt("irradianceMap", 5);
    //m_WaterPlaneShader->SetInt("prefilteredMap", 6);
    //m_WaterPlaneShader->SetInt("brdfLUT", 7);
    //
    //m_WaterPlaneShader->SetInt("sceneDepth", 4);
    //m_WaterPlaneShader->SetInt("waterNormal", 3);
    //m_WaterPlaneShader->SetInt("screenTex", 0);
    //
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, m_HDRBufffer->GetColorAttachmentID(0));
    //
    //glActiveTexture(GL_TEXTURE3);
    //glBindTexture(GL_TEXTURE_2D, m_WaterNormalTexture.GetRendererID());
    //
    //glActiveTexture(GL_TEXTURE4);
    //glBindTexture(GL_TEXTURE_2D, m_GeometryBuffer->GetDepthAttachmentID());
    //
    //glActiveTexture(GL_TEXTURE5);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, m_DiffuseIrradianceTexture.GetRendererID());
    //
    //
    //glActiveTexture(GL_TEXTURE6);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilteredTexture.GetRendererID());
    //
    //
    //glActiveTexture(GL_TEXTURE7);
    //glBindTexture(GL_TEXTURE_2D, m_brdfTexture.GetRendererID());
    //
    //
    //m_WaterPlaneMesh.RenderMesh(*m_WaterPlaneShader);












    glEnable(GL_DEPTH_TEST);


    //Copy Depth values to default frame buffer
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GeometryBuffer->GetRendererID());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_HDRBufffer->GetRendererID());


    glBlitFramebuffer(0, 0, screenRes.x, screenRes.y, 0, 0, screenRes.x, screenRes.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

    m_HDRBufffer->Bind();


    // ----- Render Skybox -------
    // change depth function so depth test passes when values are equal to depth buffer's content

    glDepthFunc(GL_LEQUAL);


    //Configure the shader
    m_CubemapShader->Use();
    m_CubemapShader->SetMat4("a_ViewMatrix", glm::mat4(glm::mat3(view))); // Strip away the translations in the matrix
    m_CubemapShader->SetMat4("a_ProjMatrix", projMatrix);

    //Render the Geometry
    m_CubemapMesh.RenderMesh(*m_CubemapShader);

    m_CrosshairShader->Use();
    m_CrosshairShader->SetVec2("u_ScreenRes", screenRes);
    m_CrosshairMesh.RenderMesh(*m_CrosshairShader);


    // Water
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   
    //glDisable(GL_BLEND);


    m_HDRBufffer->UnBind();







    glDisable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT);
    m_BloomFBO->RenderBloomTexture(m_HDRBufffer->GetColorAttachmentID(0), 0.004f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_BLEND);

    // Render to screen with tonemapping

    m_TonemappingShader->Use();
    m_TonemappingShader->SetInt("scene", 0);
    m_TonemappingShader->SetInt("bloom", 1);

    glBindVertexArray(m_CrosshairMesh.VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_HDRBufffer->GetColorAttachmentID(0));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_BloomFBO->GetBloomTexture());

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(0);

    if (InputManager::IsKeyDown(KEYCODE_ESCAPE))
        Application::Get().RequestClose();
}



WorldScene::~WorldScene()
{
    delete m_HDRBufffer;
    delete m_BloomFBO;
    delete m_GeometryBuffer;

    delete m_CubemapShader;

    delete m_CrosshairShader;

    delete m_TonemappingShader;
    delete m_WaterPlaneShader;

    Mesh::CleanUpMesh(m_CubemapMesh);
    Mesh::CleanUpMesh(m_CrosshairMesh);
    Mesh::CleanUpMesh(m_WaterPlaneMesh);
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

    m_GeometryBuffer = new GeometryBuffer();
    m_GeometryBuffer->Init(width, height);

    //m_BloomFBO_Old.Resize(width, height);
}

