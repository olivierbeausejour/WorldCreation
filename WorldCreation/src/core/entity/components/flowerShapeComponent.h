#pragma once

#include "wcpch.h"

#include "shape2dComponent.h"

static constexpr int NB_POINTS = 11;
static constexpr float ANGLE_BETWEEN_POINTS = (NB_POINTS / 2) * (360.0f / NB_POINTS);

class FlowerShapeComponent : public Shape2dComponent
{

public:

	FlowerShapeComponent() : Shape2dComponent() { }
	~FlowerShapeComponent() { }

	void setSize(const glm::vec3 size) override;

};