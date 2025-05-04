// RenderSystem.cpp: implementation of the RenderSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "RenderSystem.h"
#include "RenderComponents.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RenderSystem::RenderSystem()
{
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::Update( float dt )
{
	EntityIterator iterator(m_pEntityAdmin);
	for (auto it : iterator)
	{
		Entity* pEntity = it.second;
		TransformComponent* pTransform = pEntity->GetComponent<TransformComponent>();
		SpriteComponent* pSprite = pEntity->GetComponent<SpriteComponent>();

		if (pTransform && pSprite)
		{
			int width = 0, height = 0;
			get_image_desc(pSprite->GetSprite(), &width, &height);

			// move picture to the center
			float w = (float)width;
			float h = (float)height;
			float x = pTransform->GetPosition().x;
			float y = pTransform->GetPosition().y;
			set_obj_matrix(x, y, pTransform->GetRotation());

			x = 0.f; x = x - (w * 0.5f);
			y = 0.f; y = y - (h * 0.5f);
			draw_sprite_image(x, y, w, h, pSprite->GetSprite());
		}
	}
}
