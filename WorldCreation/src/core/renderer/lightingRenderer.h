#pragma once

#include "wcpch.h"
#include "entity/components/modelComponent.h"
#include "entity/components/lightComponent.h"
#include "entity/components/lightingModels.h"
#include "entity/entity.h"

class LightingRenderer
{

public:

	LightingRenderer();
	~LightingRenderer() = default;

	void end(ofShader* shader) { shader == nullptr ? lightingShaders[currentModel].end() : shader->end(); }

	void setLightingModel(LightingModel lightingModel) { currentModel = lightingModel; }
	LightingModel getLightingModel() { return currentModel; }

	void updateLightingShader(ofShader* shader, DrawableComponent* drawableComponent, std::vector<Entity*> lightEntities, glm::vec3 cameraPosition);

private:
	std::map<LightingModel, ofShader> lightingShaders;
	LightingModel currentModel = LightingModel::PHONG;
};