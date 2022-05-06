#include "environnementPanel.h"


EnvironnementPanel::EnvironnementPanel() 
{
	for (size_t i = 0; i < 6; i++)
	{
		cubemapImagePaths.push_back("");
	}
}

void EnvironnementPanel::draw()
{
	EditorUI::Begin("Environnement Panel", NULL);

	bool allTextureSet = true;
	if (EditorUI::CollapsingHeader("Cubemaps tools", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if(!addCubemapTextureMenu(0, "Right/Pos X texture"))  allTextureSet = false;
		if(!addCubemapTextureMenu(1, "Left/Neg X texture"))   allTextureSet = false;
		if(!addCubemapTextureMenu(2, "Top/Pos Y texture"))    allTextureSet = false;
		if(!addCubemapTextureMenu(3, "Bottom/Neg Y texture")) allTextureSet = false;
		if(!addCubemapTextureMenu(4, "Back/Pos Z texture"))   allTextureSet = false;
		if(!addCubemapTextureMenu(5, "front/Neg Z texture"))  allTextureSet = false;

		if(EditorUI::Button("Set default cubemap"))
		{
			cubemapImagePaths[0] = "resources/cubemap/default/right.jpg";
			cubemapImagePaths[1] = "resources/cubemap/default/left.jpg";
			cubemapImagePaths[2] = "resources/cubemap/default/top.jpg";
			cubemapImagePaths[3] = "resources/cubemap/default/bottom.jpg";
			cubemapImagePaths[4] = "resources/cubemap/default/front.jpg";
			cubemapImagePaths[5] = "resources/cubemap/default/back.jpg";

			allTextureSet = true;
		}

		if(allTextureSet)
		{
			ImGui::Spacing();
			EditorUI::Separator();
			if(EditorUI::Button("Load and set cubemap", {200.0f, 22.0f})) setCubemap();
		}
	}

	EditorUI::Separator();
	if (EditorUI::CollapsingHeader("Volumetric clouds tools", ImGuiTreeNodeFlags_DefaultOpen))
	{
		SceneEnvironnement* sceneEnvironnement = currentScene->getSceneEnvironnement();
		if (EditorUI::RadioButton("Activate volumetric clouds", sceneEnvironnement->getVolumetricCloudActive()))
		{
			sceneEnvironnement->setVolumetricCloudActive(!sceneEnvironnement->getVolumetricCloudActive());
		}

		if(sceneEnvironnement->getVolumetricCloudActive())
		{
			EditorUI::Text("Cloud settings");
			EditorUI::InputFloat("Cloud layer start height", sceneEnvironnement->getCloudLayerStartHeightPtr(), 0.01f, 1.0f, "%.5f");
			EditorUI::InputFloat("Cloud layer end height", sceneEnvironnement->getCloudLayerEndHeightPtr(), 0.01f, 1.0f, "%.5f");
			EditorUI::InputFloat("World diameter", sceneEnvironnement->getWorldDiameterPtr(), 0.01f, 1.0f, "%.5f");

			glm::vec3 worldOrigin = sceneEnvironnement->getWorldOrigin();
			EditorUI::Text("World origin");
			drawInputWidget("##worldOriginX", "X", &(worldOrigin.x), EditorUI::GetContentRegionAvail().x / 3.0f); EditorUI::SameLine();
			drawInputWidget("##worldOriginY", "Y", &(worldOrigin.y), EditorUI::GetContentRegionAvail().x / 2.0f); EditorUI::SameLine();
			drawInputWidget("##worldOriginZ", "Z", &(worldOrigin.z), EditorUI::GetContentRegionAvail().x);
			sceneEnvironnement->setWorldOrigin(worldOrigin);

			EditorUI::Separator();

			EditorUI::InputFloat("Density multiplier", sceneEnvironnement->getDensityMultiplierPtr(), 0.01f, 1.0f, "%.5f");
			EditorUI::InputFloat("Density offset", sceneEnvironnement->getDensityOffsetPtr(), 0.01f, 1.0f, "%.5f");
			EditorUI::InputFloat("Noise scale", sceneEnvironnement->getNoiseScalePtr(), 0.01f, 1.0f, "%.5f");
			EditorUI::InputFloat("Detail noise scale", sceneEnvironnement->getDetailNoiseScalePtr(), 0.01f, 1.0f, "%.5f");
			EditorUI::InputFloat("Detail noise weight", sceneEnvironnement->getDetailNoiseWeightPtr(), 0.01f, 1.0f, "%.5f");
			
			glm::vec3 detailWeights = sceneEnvironnement->getDetailWeights();
			EditorUI::Text("Detail weights");
			drawInputWidget("##detailWeightsX", "X", &(detailWeights.x), EditorUI::GetContentRegionAvail().x / 3.0f); EditorUI::SameLine();
			drawInputWidget("##detailWeightsY", "Y", &(detailWeights.y), EditorUI::GetContentRegionAvail().x / 2.0f); EditorUI::SameLine();
			drawInputWidget("##detailWeightsZ", "Z", &(detailWeights.z), EditorUI::GetContentRegionAvail().x);
			sceneEnvironnement->setDetailWeights(detailWeights);
			
			glm::vec4 shapeNoiseWeights = sceneEnvironnement->getShapeNoiseWeights();
			EditorUI::Text("Shape noise weights");
			drawInputWidget("##shapeNoiseWeightsX", "X", &(shapeNoiseWeights.x), EditorUI::GetContentRegionAvail().x / 4.0f); EditorUI::SameLine();
			drawInputWidget("##shapeNoiseWeightsY", "Y", &(shapeNoiseWeights.y), EditorUI::GetContentRegionAvail().x / 3.0f); EditorUI::SameLine();
			drawInputWidget("##shapeNoiseWeightsZ", "Z", &(shapeNoiseWeights.z), EditorUI::GetContentRegionAvail().x / 2.0f); EditorUI::SameLine();
			drawInputWidget("##shapeNoiseWeightsW", "W", &(shapeNoiseWeights.w), EditorUI::GetContentRegionAvail().x);
			sceneEnvironnement->setShapeNoiseWeights(shapeNoiseWeights);

			glm::vec4 phaseParams = sceneEnvironnement->getPhaseParams();
			EditorUI::Text("Phase parameters");
			drawInputWidget("##phaseParamsX", "X", &(phaseParams.x), EditorUI::GetContentRegionAvail().x / 4.0f); EditorUI::SameLine();
			drawInputWidget("##phaseParamsY", "Y", &(phaseParams.y), EditorUI::GetContentRegionAvail().x / 3.0f); EditorUI::SameLine();
			drawInputWidget("##phaseParamsZ", "Z", &(phaseParams.z), EditorUI::GetContentRegionAvail().x / 2.0f); EditorUI::SameLine();
			drawInputWidget("##phaseParamsW", "W", &(phaseParams.w), EditorUI::GetContentRegionAvail().x);
			sceneEnvironnement->setPhaseParams(phaseParams);

			EditorUI::Separator();

			glm::vec3 shapeOffset = sceneEnvironnement->getShapeOffset();
			EditorUI::Text("Shape offset");
			drawInputWidget("##shapeOffsetX", "X", &(shapeOffset.x), EditorUI::GetContentRegionAvail().x / 3.0f); EditorUI::SameLine();
			drawInputWidget("##shapeOffsetY", "Y", &(shapeOffset.y), EditorUI::GetContentRegionAvail().x / 2.0f); EditorUI::SameLine();
			drawInputWidget("##shapeOffsetZ", "Z", &(shapeOffset.z), EditorUI::GetContentRegionAvail().x);
			sceneEnvironnement->setShapeOffset(shapeOffset);

			glm::vec3 detailOffset = sceneEnvironnement->getDetailOffset();
			EditorUI::Text("Detail offset");
			drawInputWidget("##detailOffsetX", "X", &(detailOffset.x), EditorUI::GetContentRegionAvail().x / 3.0f); EditorUI::SameLine();
			drawInputWidget("##detailOffsetY", "Y", &(detailOffset.y), EditorUI::GetContentRegionAvail().x / 2.0f); EditorUI::SameLine();
			drawInputWidget("##detailOffsetZ", "Z", &(detailOffset.z), EditorUI::GetContentRegionAvail().x);
			sceneEnvironnement->setDetailOffset(detailOffset);

			EditorUI::InputFloat("Cloud move speed", sceneEnvironnement->getCloudSpeedPtr(), 0.1f, 1.0f, "%.5f");

			EditorUI::Separator();

			EditorUI::Text("Raymarch settings");
			EditorUI::InputInt("Lightmarch step count", sceneEnvironnement->getLightmarchStepCountPtr());
			EditorUI::InputInt("Raymarch step count", sceneEnvironnement->getRaymarchStepCountPtr());
			EditorUI::InputFloat("Raymarch minimum step size", sceneEnvironnement->getRaymarchMinStepSizePtr(), 0.01f, 1.0f, "%.5f");
			EditorUI::InputFloat("Ray offset strength", sceneEnvironnement->getRayOffsetStrengthPtr(), 0.01f, 1.0f, "%.5f");

			EditorUI::Separator();

			EditorUI::Text("Light settings");
			EditorUI::InputFloat("Light absorption through cloud", sceneEnvironnement->getLightAbsorptionThroughCloudPtr(), 0.01f, 1.0f, "%.5f");
			EditorUI::InputFloat("Light absorption toward sun", sceneEnvironnement->getLightAbsorptionTowardSunPtr(), 0.01f, 1.0f, "%.5f");
			EditorUI::InputFloat("Darkness threshold", sceneEnvironnement->getDarknessThresholdPtr(), 0.01f, 1.0f, "%.5f");
			
			glm::vec4 lightColor = sceneEnvironnement->getLightColor();
			EditorUI::Text("Light color");
			drawInputWidget("##lightColorX", "X", &(lightColor.x), EditorUI::GetContentRegionAvail().x / 4.0f); EditorUI::SameLine();
			drawInputWidget("##lightColorY", "Y", &(lightColor.y), EditorUI::GetContentRegionAvail().x / 3.0f); EditorUI::SameLine();
			drawInputWidget("##lightColorZ", "Z", &(lightColor.z), EditorUI::GetContentRegionAvail().x / 2.0f); EditorUI::SameLine();
			drawInputWidget("##lightColorW", "W", &(lightColor.w), EditorUI::GetContentRegionAvail().x);
			sceneEnvironnement->setLightColor(lightColor);
			
			glm::vec3 sunDir = sceneEnvironnement->getSunDir();
			EditorUI::Text("Sun direction");
			drawInputWidget("##sunDirX", "X", &(sunDir.x), EditorUI::GetContentRegionAvail().x / 3.0f); EditorUI::SameLine();
			drawInputWidget("##sunDirY", "Y", &(sunDir.y), EditorUI::GetContentRegionAvail().x / 2.0f); EditorUI::SameLine();
			drawInputWidget("##sunDirZ", "Z", &(sunDir.z), EditorUI::GetContentRegionAvail().x);
			sceneEnvironnement->setSunDir(sunDir);
		}

#ifdef WC_DEBUG
		if(EditorUI::Button("Reload shader"))
		{
			sceneEnvironnement->setupShaders();
		}
#endif
	}

	EditorUI::End();
}

bool EnvironnementPanel::addCubemapTextureMenu(int textureIndex, string hintText)
{
	bool retVal = false;
	if(cubemapImagePaths[textureIndex].compare("") == 0 && EditorUI::Button("Select", { 60.0f, 18.0f}))
	{
		auto fileChosen = ofSystemLoadDialog("Load an cubemap image file (png, jpeg, jpg)");

		if (fileChosen.bSuccess) {
			std::string fileExtension = fileChosen.fileName.substr(fileChosen.fileName.find_last_of('.') + 1);
			if (isValidFile(fileExtension))
			{
				retVal = true;
				cubemapImagePaths[textureIndex] = fileChosen.getPath();
			}
			else
			{
				EditorUI::OpenPopup("Error");
				if (EditorUI::BeginPopupModal("Error", nullptr, ImGuiWindowFlags_NoResize))
				{
					EditorUI::Text("Error reading the texture. Try to open a png, jpeg or jpg file");

					EditorUI::Spacing();
					EditorUI::SameLine(375);
					if (EditorUI::Button("OK", ImVec2(120, 0)))
						EditorUI::CloseCurrentPopup();

					EditorUI::EndPopup();
				}
			}
		}
	}

	if(cubemapImagePaths[textureIndex].compare("") != 0)
	{
		retVal = true;
		if(EditorUI::Button("Clear"))
		{
			cubemapImagePaths[textureIndex] = "";
			retVal = false;
		}
	}

	EditorUI::SameLine();
	EditorUI::Text(cubemapImagePaths[textureIndex].compare("") == 0 ? hintText.c_str() : cubemapImagePaths[textureIndex].c_str());

	return retVal;
}

void EnvironnementPanel::setCubemap()
{
	currentScene->getSceneEnvironnement()->loadCubemapTextures(cubemapImagePaths);
}

bool EnvironnementPanel::isValidFile(std::string& fileExtension) 
{
	for (int i = 0; i < fileExtension.length(); i++)
	{
		char current = fileExtension[i];
		if(std::isupper(current)) fileExtension[i] = std::tolower(current);
	}
	
	return fileExtension == "png" || fileExtension == "jpeg" || fileExtension == "jpg" || fileExtension == "gif";   
}
