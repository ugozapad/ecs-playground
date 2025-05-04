// RotateSystem.cpp: implementation of the RotateSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "RotateSystem.h"
#include "RenderComponents.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RotateSystem::RotateSystem()
{
}

RotateSystem::~RotateSystem()
{
}

void RotateSystem::Update(float dt)
{
	EntityIterator iterator(m_pEntityAdmin);
	for (auto it : iterator)
	{
		Entity* pEntity = it.second;
		TransformComponent* pTransform = pEntity->GetComponent<TransformComponent>();

		if (pTransform)
		{
			float Rotate = pTransform->GetRotation();
			Rotate += ROTATE_SPEED * dt;
			pTransform->SetRotation(Rotate);
		}
	}
}
