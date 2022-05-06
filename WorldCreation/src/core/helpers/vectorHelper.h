#pragma once

#include "wcpch.h"


class VectorHelper
{
public:

	static float distance2d(const ImVec2& v1, const ImVec2& v2)
	{
		auto distanceVec = v1 - v2;
		return sqrt(float(pow(distanceVec.x, 2)) + float(pow(distanceVec.y, 2)));
	}
};