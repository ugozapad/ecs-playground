#ifndef APPLICATION_H
#define APPLICATION_H

#include "EntityAdmin.h"
#include "RenderSystem.h"
#include "RotateSystem.h"

class Application
{
public:
    Application();
    ~Application();

    void Init();
    void Shutdown();

    void Update(float dt);

private:
    EntityAdmin m_EntityAdmin;
    RenderSystem* m_pRenderSystem;
    RotateSystem* m_pRotateSystem;
};

extern Application g_Application;

#endif // APPLICATION_H
