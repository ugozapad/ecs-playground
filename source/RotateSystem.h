// RotateSystem.h: interface for the RotateSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(ROTATESYSTEM_H__INCLUDED_)
#define ROTATESYSTEM_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EntityAdmin.h"

#define ROTATE_SPEED 50.0f

class RotateSystem : public ISystemBase
{
public:
	RotateSystem();
	~RotateSystem();

	void Update(float dt);
};

#endif // !defined(ROTATESYSTEM_H__INCLUDED_)
