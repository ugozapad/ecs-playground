#if !defined(MATHS_H)
#define MATHS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct Vec2
{
	float x, y;
};

inline Vec2 InitVec2(float x, float y)
{
	Vec2 v;
	v.x = x;
	v.y = y;
	return v;
}

#endif // !defined(MATHS_H)
