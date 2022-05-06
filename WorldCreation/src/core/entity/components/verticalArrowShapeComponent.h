#pragma once

#include "wcpch.h"

#include "shape2dComponent.h"


class VerticalArrowShapeComponent : public Shape2dComponent
{

public:

	VerticalArrowShapeComponent() : Shape2dComponent() { }
	~VerticalArrowShapeComponent() { }

	void setSize(const glm::vec3 size) override;

};