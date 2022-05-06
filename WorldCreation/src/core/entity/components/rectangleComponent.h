#pragma once

#include "wcpch.h"

#include "shape2dComponent.h"


class RectangleComponent : public Shape2dComponent
{

public:

	RectangleComponent() : Shape2dComponent() { }
	~RectangleComponent() { }

	void setSize(const glm::vec3 size) override;

};