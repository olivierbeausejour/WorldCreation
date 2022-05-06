#pragma once

#include "wcpch.h"

#include "shape2dComponent.h"


class CircleComponent : public Shape2dComponent
{

public:

	CircleComponent() : Shape2dComponent() { }
	~CircleComponent() { }

	int getCircleResolution() const { return circleResolution; }
	void setCircleResolution(const int resolution) { circleResolution = resolution; }

	void setSize(const glm::vec3 size) override;

protected:

	float circleResolution = 100;

};