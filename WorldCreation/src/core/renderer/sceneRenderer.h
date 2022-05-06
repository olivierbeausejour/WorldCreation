#pragma once

#include "wcpch.h"

#include "renderer/abstractRenderer.h"

#include "entity/entity.h"
#include "entity/components/worldComponent.h"
#include "entity/components/lightComponent.h"
#include "entity/components/metallicBallComponent.h"

#include "Scene/scene.h"

#include "interface/EditorCamera/editorCamera.h"
#include "interface/interface.h"
#include "interface/utilityPanels/toneMappingTechniques.h"
#include "renderer/postProcessingRenderer.h"
#include "renderer/lightingRenderer.h"

class Interface;


class SceneRenderer : public AbstractRenderer
{

public:

	SceneRenderer() : AbstractRenderer(1000, 1080), editorCamera(nullptr) {};
	~SceneRenderer() { };

	void setup(Scene* scene);
	void drawInternal();
	void drawEntityDrawableComponent(DrawableComponent* drawableComponent);
	void preDraw();
	void postDraw(bool isFramebufferTarget = true);
	void draw(EditorCamera* editorCamera, bool isFramebufferTarget = true);

	LightingRenderer& getLightingRenderer() { return lightingRenderer; }

	void updatePostProcessingShader(const float exposureFactor, const float gammaFactor, const ToneMappingTechnique mappingTechnique);
private:

	void drawDelimitation();
	void drawCubemap();
	void drawLights();
	void drawVolumetricClouds();

	EditorCamera* editorCamera;

	of3dPrimitive delimitationBox;
	Entity world;
	WorldComponent* worldComponent;

	bool showDelimitation;

	ofxAssimpModelLoader model;

	ofFbo preProcessingFbo;
	PostProcessingRenderer postProcessingRenderer;

	LightingRenderer lightingRenderer;

	Scene* scene;


};
