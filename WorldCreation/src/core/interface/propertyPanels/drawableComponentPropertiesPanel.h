#pragma once

#include "wcpch.h"

#include "interface/propertyPanels/componentPropertiesPanel.h"

#include "entity/components/drawableComponent.h"
#include "entity/components/modelComponent.h"
#include "entity/components/lightComponent.h"
#include "entity/components/lightTypes.h"
#include "entity/components/metallicBallComponent.h"
#include "entity/components/gradientTextureComponent.h"
#include "entity/components/imageTextureComponent.h"
#include "entity/components/renderTextureComponent.h"
#include "entity/components/filterComponent.h"
#include "entity/components/grayscaleFilterComponent.h"
#include "entity/components/sepiaFilterComponent.h"
#include "entity/components/invertFilterComponent.h"
#include "entity/components/bezierSurfaceComponent.h"

#include "inputs/actions.h"

class Interface;

class DrawableComponentPropertiesPanel : public ComponentPropertiesPanel
{

public:

	DrawableComponentPropertiesPanel() 
	{
		viewportNames.push_back(TYPE_NONE);
		viewports.insert(std::make_pair<string, AbstractViewport*>(TYPE_NONE, nullptr));
	};
	~DrawableComponentPropertiesPanel() {};

	void setup(Scene* scene) 
	{
		this->scene = scene; 
	}
	void draw() override;

	void addViewport(AbstractViewport* viewport);
	void removeViewport(AbstractViewport* viewport);

private:
	
	void drawLightSection(LightComponent* lightComponent);
	void drawSectionModel(ModelComponent* modelComponent);
	void drawSectionMetallicBall(MetallicBallComponent* MetallicBallComponent);
	void drawSectionColor();
	void drawSectionRenderMode();
	void drawSectionTexture();
	void drawSectionFilter();
	void drawSectionControlPoints(BezierSurfaceComponent* surfaceComponent);

	void loadModelFromFile(Action& menuAction, ModelComponent* modelComponent, string modelPath);
	bool loadImageFromFile(Action& menuAction);

	int selectedAnimation = 0;

	float roughness = 0.0f;
	float normalValue = 1.0f;
	glm::vec4 color;

	static constexpr char* TYPE_NONE = "None";

	static constexpr char* TYPE_TEXTURE_GRADIENT = "Gradient";
	static constexpr char* TYPE_TEXTURE_IMAGE = "Image";

	static constexpr char* textureTypes[3] = { TYPE_NONE, TYPE_TEXTURE_GRADIENT, TYPE_TEXTURE_IMAGE };
	int selectedTextureType = 0;

	Scene* scene;

	static constexpr char* TYPE_GRADIENT_HORIZONTAL = "Horizontal";
	static constexpr char* TYPE_GRADIENT_VERTICAL = "Vertical";

	static constexpr char* gradientTypes[2] = { TYPE_GRADIENT_HORIZONTAL, TYPE_GRADIENT_VERTICAL };
	int selectedGradientType = 0;
	glm::vec4 gradientBeginColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 gradientEndColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	char selectedImagePath[255] = "";
	ofImage* selectedImage = nullptr;

	vector<string> viewportNames;
	map<string, AbstractViewport*> viewports;
	const char* selectedViewportName = TYPE_NONE;
	string selectedViewportNameStr = "";
	AbstractViewport* selectedViewport = nullptr;

	static constexpr char* TYPE_FILTER_GRAYSCALE = "Grayscale";
	static constexpr char* TYPE_FILTER_SEPIA = "Sepia";
	static constexpr char* TYPE_FILTER_INVERT = "Invert";

	static constexpr char* filterTypes[4] = { TYPE_NONE, TYPE_FILTER_GRAYSCALE, TYPE_FILTER_SEPIA, TYPE_FILTER_INVERT };
	int selectedFilterType = 0;

};

struct ModelImport
{
	ofxAssimpModelLoader* modelPtr = nullptr;
	string modelName = "";
};

struct ImageImport
{
	ofImage* imagePtr = nullptr;
	string imagePath = "";
};
