#ifndef RENDERCOMPONENTS_H
#define RENDERCOMPONENTS_H

#include <maths.h>
#include <engine.h>
#include <EntityAdmin.h>

class TransformComponent : public IComponentBase
{
public:
    TransformComponent();
    ~TransformComponent();

    const Vec2&         GetPosition()                           { return m_Position; }
    void                SetPosition(const Vec2& position)       { m_Position = position; }
    void                SetPosition(float x, float y)           { m_Position.x = x; m_Position.y = y; }

    const float     	GetRotation()                           { return m_Rotation; }
    void                SetRotation(const float Rotation)   	{ m_Rotation = Rotation; }

private:
    Vec2 m_Position;
    float m_Rotation;
};

class SpriteComponent : public IComponentBase
{
public:
    SpriteComponent();
    ~SpriteComponent();

    void LoadSprite(const char* pFilename);

    void SetSprite(image_handle image) { m_Image = image; }
    const image_handle GetSprite() const { return m_Image; }

private:
    image_handle m_Image;

};

#endif // RENDERCOMPONENTS_H
