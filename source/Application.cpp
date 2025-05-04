#include <stdio.h>
#include <time.h>
#include "engine.h"
#include "Application.h"
#include "RenderComponents.h"

namespace profiler
{
    float g_FrameTime = 0.0f;
    float g_RenderSystemTime = 0.0f;
    float g_RotateSystemTime = 0.0f;
}

Application g_Application;

Application::Application() :
    m_pRenderSystem(NULL),
    m_pRotateSystem(NULL)
{
    //ctor
}

Application::~Application()
{
    m_pRotateSystem = NULL;
    m_pRenderSystem = NULL;
}

void Application::Init()
{
    // Create render system
    m_pRenderSystem = m_EntityAdmin.CreateSystem< RenderSystem >();

    // Create rotate system 
    m_pRotateSystem = m_EntityAdmin.CreateSystem< RotateSystem >();

    // Load our sprite
    image_handle sprite = load_image("resources/sprite1.bmp");

    const int RandomEntityNum = 250;//100;
    
    // Create random entities
    for (int i = 0; i < RandomEntityNum; i++)
    {
        // Create entity
        Entity* pEntity = m_EntityAdmin.CreateEntity();

        // Create transform component and randomize angle and position
        TransformComponent* pTransform = pEntity->CreateComponent<TransformComponent>();

        // Randomize angle
        float Angle = (float)rand() / (float)RAND_MAX;

        float xmax = get_widthf() * 0.95f;
        float ymax = get_heightf() * 0.95f;
        float xmin = get_widthf() * 0.05f;
        float ymin = get_heightf() * 0.05f;

        // Randomize position
        float x = ((float)rand() / (float)RAND_MAX) * (xmax - xmin) + xmin;
        float y = ((float)rand() / (float)RAND_MAX) * (ymax - ymin) + ymin;

        pTransform->SetPosition(x, y);
        pTransform->SetRotation(Angle);

        SpriteComponent* pSprite = pEntity->CreateComponent<SpriteComponent>();
        pSprite->SetSprite(sprite);
    }  
}

void Application::Shutdown()
{
    m_EntityAdmin.RemoveEntities();
}

void Application::Update(float dt)
{
    static char debugString[128];
    sprintf(debugString, "Entity num: %i\nTime %.4f ms\nRotate  %.4f ms\nRender  %.4f ms", 
        m_EntityAdmin.GetEntities().size(),
        profiler::g_FrameTime,
        profiler::g_RotateSystemTime,
        profiler::g_RenderSystemTime
    );
    
    set_debug_text(debugString);

    float beginTime = get_time();

    reset_state();
    set_2d_matrix(0.f, 0.f, get_widthf(), get_heightf());

    float rotateBeginTime = get_time();
    m_pRotateSystem->Update(dt);
    profiler::g_RotateSystemTime = get_time() - rotateBeginTime;

    float renderBeginTime = get_time();
    m_pRenderSystem->Update(dt);
    profiler::g_RenderSystemTime = get_time() - renderBeginTime;

    profiler::g_FrameTime = get_time() - beginTime;
}

// Callback for sokol app
extern "C"
{

void app_init()
{
    printf("app_init\n");

    srand(time(NULL));

    g_Application.Init();
}

void app_shutdown()
{
    printf("app_shutdown\n");

    g_Application.Shutdown();
}

void app_update(float dt)
{
    printf("app_update %f\n", dt);

    g_Application.Update(dt);
}

}
