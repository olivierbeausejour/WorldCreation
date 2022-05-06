#pragma once

#include "wcpch.h"
#include "interface/propertyPanels/componentPropertiesPanel.h"
#include "entity/components/lightingModels.h"
#include "toneMappingTechniques.h"


class SceneUtilityPanel
{

public:

	SceneUtilityPanel() { };
	~SceneUtilityPanel() { };

	void setup(EditorUI::Gui* gui);
	void draw();

	bool getIsScreenCapturing() const;
	void postScreenCapture();

	float getExposureFactor() const;
	float getGammaFactor() const;
	ToneMappingTechnique getToneMappingTechnique() const;
	LightingModel getLightingModel() const;

	string getOutputPath() const { return string(outputPath); }

private:

	ofImage screenshotButtonImage;
	GLuint screenshotButtonImageId;

	ofImage screenshotButtonDisabledImage;
	GLuint screenshotButtonDisabledImageId;

	bool screenShotButtonEnabled = true;

	ofImage recordButtonImage;
	GLuint recordButtonImageId;

	ofImage recordButtonDisabledImage;
	GLuint recordButtonDisabledImageId;

	bool recordButtonEnabled = true;

	ofImage stopButtonImage;
	GLuint stopButtonImageId;

	ofImage stopButtonDisabledImage;
	GLuint stopButtonDisabledImageId;

	bool stopButtonEnabled = false;

	bool takingScreenshot = false;
	bool takingVideoCapture = false;
	int fps = 1;
	uint64_t lastVideoCaptureTime;

	char outputPath[255] = "";
	bool validOutputPath = false;

	float exposureFactor = 1;
	float gammaFactor = 2.2f;
	int toneMappingTechnique = 1;
	int lightingModel = 2;
};