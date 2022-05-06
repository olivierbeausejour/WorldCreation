#pragma once

#include "wcpch.h"

#include "shape2dComponent.h"


class HorizontalArrowShapeComponent : public Shape2dComponent
{

public:

	HorizontalArrowShapeComponent() : Shape2dComponent() { }
	~HorizontalArrowShapeComponent() { }

	void setSize(const glm::vec3 size) override;

};