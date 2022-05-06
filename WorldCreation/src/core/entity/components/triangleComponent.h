#pragma once

#include "wcpch.h"

#include "shape2dComponent.h"


class TriangleComponent : public Shape2dComponent
{

public:

	TriangleComponent() : Shape2dComponent() { }
	~TriangleComponent() { }

	void setSize(const glm::vec3 size) override;

};