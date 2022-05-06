#include "lightComponent.h"

const int LightComponent::MAX_NUMBER_OF_LIGHTS = 8;

LightComponent::LightComponent(const LightType lightType, const bool drawLight): DrawableComponent() 
{ 
	this->lightType = lightType; 
	color = ofColor(255); 
	mustDraw = drawLight;

	setupLightProperties();
}

void LightComponent::setupLightProperties()
{
	switch (lightType)
	{
		case LightType::AMBIENT:
			break;
		case LightType::DIRECTIONAL:
			light.setDirectional();
			break;
		case LightType::POINT:
			light.setPointLight();
			break;
		case LightType::SPOT:
			light.setSpotlight();
			light.setSpotConcentration(4.0f);
			break;
	}
}

void LightComponent::draw() 
{
	if (mustDraw && lightType != LightType::AMBIENT)
	{
		light.resetTransform();

		preDraw();

		light.setGlobalOrientation(transform->getGlobalRotation());

		light.setPosition(transform->getGlobalPosition());

		switch (lightType)
		{
			case LightType::DIRECTIONAL:
				light.setScale(0.1f * transform->getLocalScale().x, -0.1f * transform->getLocalScale().y, 0.1f * transform->getLocalScale().z);
				break;
			case LightType::POINT:
				light.setScale(0.05f * transform->getLocalScale().x, -0.05f * transform->getLocalScale().y, 0.05f * transform->getLocalScale().z);
				break;
			case LightType::SPOT:
				light.setScale(0.02f * transform->getLocalScale().x, 0.02f * transform->getLocalScale().y, 0.02f * transform->getLocalScale().z);
				break;
		}
		
		light.draw();

		postDraw();
	}
}