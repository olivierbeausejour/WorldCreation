#include "sceneUtilityPanel.h"

void SceneUtilityPanel::setup(EditorUI::Gui* gui)
{
    screenshotButtonImage.load("editor/icons/button_screenshot.png");
    screenshotButtonImageId = gui->loadPixels(screenshotButtonImage);

    screenshotButtonDisabledImage.load("editor/icons/button_screenshot_disabled.png");
    screenshotButtonDisabledImageId = gui->loadPixels(screenshotButtonDisabledImage);

    recordButtonImage.load("editor/icons/button_record.png");
    recordButtonImageId = gui->loadPixels(recordButtonImage);

    recordButtonDisabledImage.load("editor/icons/button_record_disabled.png");
    recordButtonDisabledImageId = gui->loadPixels(recordButtonDisabledImage);

    stopButtonImage.load("editor/icons/button_stop.png");
    stopButtonImageId = gui->loadPixels(stopButtonImage);

    stopButtonDisabledImage.load("editor/icons/button_stop_disabled.png");
    stopButtonDisabledImageId = gui->loadPixels(stopButtonDisabledImage);
}

void SceneUtilityPanel::draw()
{
	EditorUI::Begin("Scene Tools", NULL);

	if (EditorUI::CollapsingHeader("Tone Mapping", ImGuiTreeNodeFlags_DefaultOpen)) 
	{
		for (int i = 1; i <= 4; i++) EditorUI::Spacing();
		ComponentPropertiesPanel::drawInputWidget("##ExpoFactor", "Exposure Factor", &exposureFactor, EditorUI::GetContentRegionAvail().x, 0.1f, 0.5f);
		exposureFactor = ofClamp(exposureFactor, 0.0f, 30.0f);

		for (int i = 1; i <= 2; i++) EditorUI::Spacing();
		ComponentPropertiesPanel::drawInputWidget("##GammaFactor", "Gamma Factor", &gammaFactor, EditorUI::GetContentRegionAvail().x, 0.1f, 0.5f);
		gammaFactor = ofClamp(gammaFactor, 0.0f, 5.0f);

		for (int i = 1; i <= 2; i++) EditorUI::Spacing();
		EditorUI::LabelText("", "Mapping Technique");
        EditorUI::RadioButton("Reinhard", &toneMappingTechnique, 0);
        EditorUI::RadioButton("ACES Filmic", &toneMappingTechnique, 1);
		for (int i = 1; i <= 2; i++) EditorUI::Spacing();
	}

	if (EditorUI::CollapsingHeader("Lighting", ImGuiTreeNodeFlags_DefaultOpen)) 
	{
		for (int i = 1; i <= 2; i++) EditorUI::Spacing();
		EditorUI::LabelText("", "Lighting Model");
        EditorUI::RadioButton("Lambert", &lightingModel, 0);
    	EditorUI::RadioButton("Phong", &lightingModel, 1);
    	EditorUI::RadioButton("Blinn-Phong", &lightingModel, 2);
		for (int i = 1; i <= 2; i++) EditorUI::Spacing();
	}

	if (EditorUI::CollapsingHeader("Scene Capture")) 
	{
		EditorUI::LabelText("", "Screenshot");

		EditorUI::PushItemFlag(ImGuiItemFlags_Disabled, !screenShotButtonEnabled || !validOutputPath);
		if (EditorUI::ImageButton(GetImTextureID(screenShotButtonEnabled && validOutputPath ? screenshotButtonImageId : screenshotButtonDisabledImageId), ImVec2(32, 32)))
		{
			takingScreenshot = true;
		}
		EditorUI::PopItemFlag();

		EditorUI::Separator();

		EditorUI::LabelText("", "Screen Record");

		EditorUI::PushItemFlag(ImGuiItemFlags_Disabled, !recordButtonEnabled || !validOutputPath);
		if (EditorUI::ImageButton(GetImTextureID(recordButtonEnabled && validOutputPath ? recordButtonImageId : recordButtonDisabledImageId), ImVec2(32, 32)))
		{
			screenShotButtonEnabled = false;
			recordButtonEnabled = false;
			stopButtonEnabled = true;

			takingVideoCapture = true;
			lastVideoCaptureTime = ofGetSystemTimeMillis();
		}
		EditorUI::PopItemFlag();

		EditorUI::SameLine();

		EditorUI::PushItemFlag(ImGuiItemFlags_Disabled, !stopButtonEnabled || !validOutputPath);
		if (EditorUI::ImageButton(GetImTextureID(stopButtonEnabled && validOutputPath ? stopButtonImageId : stopButtonDisabledImageId), ImVec2(32, 32)))
		{
			screenShotButtonEnabled = true;
			recordButtonEnabled = true;
			stopButtonEnabled = false;

			takingVideoCapture = false;
		}
		EditorUI::PopItemFlag();

		EditorUI::PushItemFlag(ImGuiItemFlags_Disabled, !recordButtonEnabled || !validOutputPath);
		EditorUI::DragInt("Framerate", &fps, 0.1f, 1, 30);
		EditorUI::PopItemFlag();

		EditorUI::Separator();

		EditorUI::LabelText("", "Output Path");

		if (EditorUI::Button("Choose"))
		{
			auto fileChosen = ofSystemLoadDialog("Select an output directory", true);
			validOutputPath = fileChosen.bSuccess;

			if (validOutputPath)
				strcpy(outputPath, fileChosen.getPath().c_str());
		}
		EditorUI::SameLine();
		EditorUI::LabelText("", outputPath);
	}

    EditorUI::End();
}

bool SceneUtilityPanel::getIsScreenCapturing() const
{
    return takingScreenshot || (takingVideoCapture && (ofGetSystemTimeMillis() - lastVideoCaptureTime) / 1000.0f >= 1.0f / (float)fps);
}

void SceneUtilityPanel::postScreenCapture()
{
    takingScreenshot = false;
    lastVideoCaptureTime = ofGetSystemTimeMillis();
}

float SceneUtilityPanel::getExposureFactor() const
{
	return exposureFactor;
}

float SceneUtilityPanel::getGammaFactor() const
{
	return gammaFactor;
}

ToneMappingTechnique SceneUtilityPanel::getToneMappingTechnique() const
{
	return static_cast<ToneMappingTechnique>(toneMappingTechnique);
}

LightingModel SceneUtilityPanel::getLightingModel() const
{
	return static_cast<LightingModel>(lightingModel);
}
