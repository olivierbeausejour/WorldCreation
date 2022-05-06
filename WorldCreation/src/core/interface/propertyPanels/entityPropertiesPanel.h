#pragma once

#include "entity/entity.h"
#include "entity/components/transformComponent.h"
#include "entity/components/drawableComponent.h"
#include "entity/components/cubeComponent.h"
#include "entity/components/coneComponent.h"
#include "entity/components/lightComponent.h"
#include "entity/components/lightTypes.h"
#include "entity/components/planeComponent.h"
#include "entity/components/sphereComponent.h"
#include "entity/components/metallicBallComponent.h"
#include "entity/components/modelComponent.h"
#include "entity/components/bezierSurfaceComponent.h"
#include "entity/components/renderTextureComponent.h"
#include "interface/propertyPanels/transformComponentPropertiesPanel.h"
#include "interface/propertyPanels/drawableComponentPropertiesPanel.h"
#include "interface/propertyPanels/materialComponentPropertiesPanel.h"
#include "Scene/scene.h"

class Interface;

class EntityPropertiesPanel
{

public:
	EntityPropertiesPanel()
	{
		this->transformPanel = new TransformComponentPropertiesPanel();
		this->drawablePanel = new DrawableComponentPropertiesPanel();
		this->materialPanel = new MaterialComponentPropertiesPanel();
	}
	~EntityPropertiesPanel()
	{
		delete transformPanel;
		delete drawablePanel;
		delete materialPanel;
	}

	void setup(Scene *scene);
	void draw();

	DrawableComponentPropertiesPanel *getDrawableComponentPropertiesPanel() { return drawablePanel; }

private:
	Scene *scene = nullptr;

	TransformComponentPropertiesPanel* transformPanel;
	DrawableComponentPropertiesPanel* drawablePanel;
	MaterialComponentPropertiesPanel* materialPanel;

	void AddPopupContext(Entity* currentEntity = nullptr);
};
