#pragma once

#include "wcpch.h"
#include "entity/components/drawableComponent.h"
#include "entity/components/lightTypes.h"

class LightComponent : public DrawableComponent
{
public:
	static const int MAX_NUMBER_OF_LIGHTS;

	LightComponent(const LightType lightType, const bool drawLight = true);
	~LightComponent() = default;

	void draw() override;

	LightType getType() const { return lightType; }
	ofLight getLight() const { return light; }
	float getSpecularShining() const { return specularShining; }
	void setSpecularShining(const float specularShining) { this->specularShining = specularShining; }
	bool* showInScene() { return &mustDraw; }

private:
	ofLight light;
	float specularShining = 32;
	LightType lightType;
	bool mustDraw;

	void setupLightProperties();
};