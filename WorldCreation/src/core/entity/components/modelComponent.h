#pragma once

#include "wcpch.h"

#include "drawableComponent.h"


class ModelComponent : public DrawableComponent
{

public:

	ModelComponent::ModelComponent() : model(nullptr), modelLoaded(false), animationPlaying(false) {}
	~ModelComponent();

	void setModel(ofxAssimpModelLoader* model);
	bool getModelLoaded() { return modelLoaded; }
	void unloadModel();

	void update();
	void draw() override;
	bool hasAnimation();
	int getAnimationCount();
	void playAnimation(int animIndex);
	void pauseAnimation(int animIndex);
	void setAnimation(int animIndex);
	bool getAnimationPlaying() { return animationPlaying; }
	string getModelName() { return modelName; }
	bool mustInvertNormal() { return invertNormal; }
	void setModelName(string modelName) { this->modelName = modelName; }


private:

	ofxAssimpModelLoader* model;
	bool modelLoaded;
	bool invertNormal;
	bool animationPlaying;
	string modelName;

};