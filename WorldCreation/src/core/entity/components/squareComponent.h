#pragma once

#include "wcpch.h"

#include "shape2dComponent.h"


class SquareComponent : public Shape2dComponent
{

public:

	SquareComponent() : Shape2dComponent() { }
	~SquareComponent() { }

	void setSize(const glm::vec3 size) override;

};