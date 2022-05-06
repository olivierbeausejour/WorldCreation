#include "modelComponent.h"


ModelComponent::~ModelComponent()
{
	delete model;
	model = nullptr;
}

void ModelComponent::setModel(ofxAssimpModelLoader* model) 
{
	this->model = model; 
	modelLoaded = true; 
	animationPlaying = false; 

	this->model->enableTextures();
	this->model->disableMaterials();

	materialComponent.setAmbientColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
	materialComponent.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
	
	invertNormal = modelName.find("pot") != string::npos; // The teapot model has inverted normals, that's why.
}

void ModelComponent::unloadModel()
{
	modelLoaded = false;
	if (model != nullptr) delete model; 
	model = nullptr;
}

void ModelComponent::update()
{
	if (model != nullptr)
	{
		model->update();
	}
}

void ModelComponent::draw()
{
	if (model == nullptr) return;
	TransformComponent transform = *DrawableComponent::getTransform();
	glm::vec3 position = transform.getGlobalPosition();
	glm::vec3 eulerAngles = transform.getGlobalEulerAngles();
	glm::vec3 scale = transform.getGlobalScale();
	model->setPosition(position.x, position.y, position.z);
	model->setRotation(0, eulerAngles.x, 1.0f, 0.0f, 0.0f);
	model->setRotation(1, eulerAngles.y, 0.0f, 1.0f, 0.0f);
	model->setRotation(2, eulerAngles.z, 0.0f, 0.0f, 1.0f);
	model->setScale(scale.x * 0.001f, scale.y * 0.001f, scale.z * 0.001f);
	switch (DrawableComponent::getRenderMode())
	{
	case ofPolyRenderMode::OF_MESH_FILL:
		model->drawFaces();
		break;
	case ofPolyRenderMode::OF_MESH_POINTS:
		model->drawVertices();
		break;
	case ofPolyRenderMode::OF_MESH_WIREFRAME:
		model->drawWireframe();
		break;
	}
}

bool ModelComponent::hasAnimation()
{
	if (modelLoaded)
	{
		return model->hasAnimations();
	}
}

int ModelComponent::getAnimationCount()
{
	if (modelLoaded)
	{
		return model->getAnimationCount();
	}
}

void ModelComponent::playAnimation(int animIndex)
{
	model->getAnimation(animIndex).play();
	animationPlaying = true;
}

void ModelComponent::pauseAnimation(int animIndex)
{
	if (model->getAnimation(animIndex).isPlaying())
	{
		model->getAnimation(animIndex).setPaused(true);
		animationPlaying = false;
	}
}

void ModelComponent::setAnimation(int animIndex)
{
	model->setAnimation(animIndex);
}