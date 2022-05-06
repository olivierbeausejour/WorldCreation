#pragma once

#include "wcpch.h"
#include "interface/propertyPanels/componentPropertiesPanel.h"
#include "entity/components/drawableComponent.h"
#include "entity/components/bezierSurfaceComponent.h"
#include "entity/components/metallicBallComponent.h"

class Interface;

class MaterialComponentPropertiesPanel : public ComponentPropertiesPanel
{

public:

	MaterialComponentPropertiesPanel() = default;
	~MaterialComponentPropertiesPanel() = default;

	void draw() override;

private:

	void drawMaterialSection(DrawableComponent* drawableComponent);

};