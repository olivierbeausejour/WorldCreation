#pragma once

#include "wcpch.h"

#include "abstractViewport.h"
#include "customCursor.h"

#include "inputs/mouseCommands.h"

#include "helpers/vectorHelper.h"

class CanvasViewport : public AbstractViewport
{

public:

	CanvasViewport() : AbstractViewport(1000, 1080, "Canvas") { };
	~CanvasViewport() { };

	void draw() override;
};
