// RenderSystem.h: interface for the RenderSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(RENDERSYSTEM_H__INCLUDED_)
#define RENDERSYSTEM_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EntityAdmin.h"

class RenderSystem : public ISystemBase
{
public:
	RenderSystem();
	~RenderSystem();

	void Update( float dt );
};

#endif // !defined(RENDERSYSTEM_H__INCLUDED_)
