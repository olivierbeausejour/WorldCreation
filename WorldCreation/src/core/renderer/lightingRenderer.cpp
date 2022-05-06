#include "lightingRenderer.h"

LightingRenderer::LightingRenderer()
{ 
	lightingShaders[LightingModel::LAMBERT].load("resources/shader/lighting/lambert/lambert.vert", "resources/shader/lighting/lambert/lambert.frag"); 
	lightingShaders[LightingModel::PHONG].load("resources/shader/lighting/phong/phong.vert", "resources/shader/lighting/phong/phong.frag"); 
	lightingShaders[LightingModel::BLINN_PHONG].load("resources/shader/lighting/blinn_phong/blinn_phong.vert", "resources/shader/lighting/blinn_phong/blinn_phong.frag"); 
}

void LightingRenderer::updateLightingShader(ofShader* shader, DrawableComponent* drawableComponent, 
											std::vector<Entity*> lightEntities, glm::vec3 cameraPosition)
{
	ofShader* lightingShader;
	if (shader == nullptr)
		lightingShader = &lightingShaders[currentModel];
	else
		lightingShader = shader;

	lightingShader->begin();

	std::vector<glm::vec3> lightPositions; lightPositions.reserve(LightComponent::MAX_NUMBER_OF_LIGHTS);
	std::vector<glm::vec3> lightDirections;	lightDirections.reserve(LightComponent::MAX_NUMBER_OF_LIGHTS);

	std::vector<ofFloatColor> lightAmbientColors; lightAmbientColors.reserve(LightComponent::MAX_NUMBER_OF_LIGHTS);
	std::vector<ofFloatColor> lightDiffuseColors; lightDiffuseColors.reserve(LightComponent::MAX_NUMBER_OF_LIGHTS);
	std::vector<ofFloatColor> lightSpecularColors; lightSpecularColors.reserve(LightComponent::MAX_NUMBER_OF_LIGHTS);
	std::vector<float> lightSpecularFactors; lightSpecularFactors.reserve(LightComponent::MAX_NUMBER_OF_LIGHTS);

	std::vector<float> lightConstantAttenuations; lightConstantAttenuations.reserve(LightComponent::MAX_NUMBER_OF_LIGHTS);
	std::vector<float> lightLinearAttenuations; lightLinearAttenuations.reserve(LightComponent::MAX_NUMBER_OF_LIGHTS);
	std::vector<float> lightQuadraticAttenuations; lightQuadraticAttenuations.reserve(LightComponent::MAX_NUMBER_OF_LIGHTS);
	std::vector<float> lightAttenuationPowers; lightAttenuationPowers.reserve(LightComponent::MAX_NUMBER_OF_LIGHTS);
	std::vector<int> lightTypes; lightTypes.reserve(LightComponent::MAX_NUMBER_OF_LIGHTS);

	lightingShader->setUniform4f("u_materialAmbientColor", drawableComponent->getMaterial().getAmbientColor());
	lightingShader->setUniform4f("u_materialDiffuseColor", drawableComponent->getMaterial().getDiffuseColor());
	lightingShader->setUniform4f("u_materialSpecularColor", drawableComponent->getMaterial().getSpecularColor());

	if (ModelComponent* model = dynamic_cast<ModelComponent*>(drawableComponent))
	{
		lightingShader->setUniform1i("u_invertNormal", (int) model->mustInvertNormal());
		lightingShader->setUniform1i("u_isModel", 1);
	}
	else
	{
		lightingShader->setUniform1i("u_invertNormal", 0);
		lightingShader->setUniform1i("u_isModel", 0);
	}

	// Ajout des lumières de la scène
	for(size_t i = 0; i < lightEntities.size(); i++)
	{
		std::string strIndex = ofToString(i);

		// Pris à partir du fichier ofLight.cpp, méthode "onOrientationChanged"
		glm::vec3 direction(glm::normalize(lightEntities[i]->getTransform()->getGlobalRotation() * glm::vec4(0, 0, -1, 1)));

		glm::vec3 lightPosition = lightEntities[i]->getTransform()->getGlobalPosition();

		LightComponent* lightComponent = dynamic_cast<LightComponent*>(lightEntities[i]->getDrawableComponent());

		lightPositions.push_back(lightPosition);
		lightDirections.push_back(direction);
		lightAmbientColors.push_back(lightComponent->getLight().getAmbientColor());
		lightDiffuseColors.push_back(lightComponent->getLight().getDiffuseColor());
		lightSpecularColors.push_back(lightComponent->getLight().getSpecularColor());
		lightSpecularFactors.push_back(lightComponent->getSpecularShining());
		lightTypes.push_back(static_cast<int>(lightComponent->getType()));

		lightConstantAttenuations.push_back(lightComponent->getLight().getAttenuationConstant());
		lightLinearAttenuations.push_back(lightComponent->getLight().getAttenuationLinear());
		lightQuadraticAttenuations.push_back(lightComponent->getLight().getAttenuationQuadratic());

		if (lightComponent->getType() == LightType::SPOT)
			lightAttenuationPowers.push_back(lightComponent->getLight().getSpotConcentration());
		else
			lightAttenuationPowers.push_back(0.0f);
	}

	lightingShader->setUniform3fv("u_lightPositions", &lightPositions.data()[0].x, LightComponent::MAX_NUMBER_OF_LIGHTS);
	lightingShader->setUniform3fv("u_lightDirections", &lightDirections.data()[0].x, LightComponent::MAX_NUMBER_OF_LIGHTS);
	lightingShader->setUniform4fv("u_lightAmbientColors", &lightAmbientColors.data()[0].r, LightComponent::MAX_NUMBER_OF_LIGHTS);
	lightingShader->setUniform4fv("u_lightDiffuseColors", &lightDiffuseColors.data()[0].r, LightComponent::MAX_NUMBER_OF_LIGHTS);
	lightingShader->setUniform4fv("u_lightSpecularColors", &lightSpecularColors.data()[0].r, LightComponent::MAX_NUMBER_OF_LIGHTS);
	lightingShader->setUniform1fv("u_lightSpecularFactors", &lightSpecularFactors.data()[0], LightComponent::MAX_NUMBER_OF_LIGHTS);

	lightingShader->setUniform1fv("u_lightConstantAttenuations", &lightConstantAttenuations.data()[0], LightComponent::MAX_NUMBER_OF_LIGHTS);
	lightingShader->setUniform1fv("u_lightLinearAttenuations", &lightLinearAttenuations.data()[0], LightComponent::MAX_NUMBER_OF_LIGHTS);
	lightingShader->setUniform1fv("u_lightQuadraticAttenuations", &lightQuadraticAttenuations.data()[0], LightComponent::MAX_NUMBER_OF_LIGHTS);
	lightingShader->setUniform1fv("u_lightAttenuationPowers", &lightAttenuationPowers.data()[0], LightComponent::MAX_NUMBER_OF_LIGHTS);

	lightingShader->setUniform1iv("u_lightTypes", &lightTypes.data()[0], LightComponent::MAX_NUMBER_OF_LIGHTS);
	lightingShader->setUniform1i("u_numberOfLights", lightEntities.size());
	lightingShader->setUniform3f("u_cameraPosition", cameraPosition);
}