#pragma once

#include "wcpch.h"

#include "shape2dComponent.h"


class RightTriangleComponent : public Shape2dComponent
{

public:

	RightTriangleComponent() : Shape2dComponent() { }
	~RightTriangleComponent() { }

	void setSize(const glm::vec3 size) override;

};