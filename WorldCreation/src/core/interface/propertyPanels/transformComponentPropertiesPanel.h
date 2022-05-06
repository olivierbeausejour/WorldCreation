#pragma once
#include "wcpch.h"
#include "interface/propertyPanels/componentPropertiesPanel.h"
#include "entity/components/transformComponent.h"
#include "helpers/mathHelper.h"

class Interface;

class TransformComponentPropertiesPanel : public ComponentPropertiesPanel
{

public:

	TransformComponentPropertiesPanel()
	{
	}

	~TransformComponentPropertiesPanel()
	{
	}	

	void draw() override;

private:

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	int showAsCartesian = 1;
};

