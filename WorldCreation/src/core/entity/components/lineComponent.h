#pragma once

#include "wcpch.h"

#include "shape2dComponent.h"
#include "lineDrawer.hpp"

class LineComponent : public Shape2dComponent
{

public:
	LineComponent() : Shape2dComponent() {}
	~LineComponent() {}

	void draw() override;

private:
	LineDrawer lineDrawer;
};