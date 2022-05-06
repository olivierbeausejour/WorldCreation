#include "drawableComponentPropertiesPanel.h"
#include "interface/interface.h"

void DrawableComponentPropertiesPanel::draw()
{
    if (Interface::getSelectedEntity() != nullptr &&
        Interface::getSelectedEntity()->getDrawableComponent() != nullptr)
    {
        if (ModelComponent *modelComponent = dynamic_cast<ModelComponent *>(Interface::getSelectedEntity()->getDrawableComponent()))
        {
            if (EditorUI::CollapsingHeader("Model", ImGuiTreeNodeFlags_DefaultOpen))
            {
                drawSectionModel(modelComponent);
            }
        }
		else if (LightComponent* lightComponent = dynamic_cast<LightComponent*>(Interface::getSelectedEntity()->getDrawableComponent()))
        {
            if (EditorUI::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
            {
                drawLightSection(lightComponent);
			}
		}
        else if (MetallicBallComponent* metallicBallComponent = dynamic_cast<MetallicBallComponent*>(Interface::getSelectedEntity()->getDrawableComponent()))
        {
            if (EditorUI::CollapsingHeader("Metal Ball", ImGuiTreeNodeFlags_DefaultOpen))
            {
                drawSectionColor();
                drawSectionRenderMode();
                drawSectionMetallicBall(metallicBallComponent);
            }
        }
        else
        {
            if (EditorUI::CollapsingHeader("Drawable", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (BezierSurfaceComponent *surfaceComponent = dynamic_cast<BezierSurfaceComponent *>(Interface::getSelectedEntity()->getDrawableComponent()))
                {
                    drawSectionControlPoints(surfaceComponent);
                }
				else
				{
					drawSectionRenderMode();
                	drawSectionTexture();
                	drawSectionFilter();
				}
            }
        }
    }
}

void DrawableComponentPropertiesPanel::drawLightSection(LightComponent* lightComponent)
{
	glm::vec4 ambientColor = glm::vec4
    (
        lightComponent->getLight().getAmbientColor().r,
        lightComponent->getLight().getAmbientColor().g,
        lightComponent->getLight().getAmbientColor().b,
        lightComponent->getLight().getAmbientColor().a
    );

	EditorUI::LabelText("", "Ambient Color");
	EditorUI::ColorPicker4("##AmbientColor", (float*)&ambientColor, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel);
	for (int i = 1; i <= 2; i++) EditorUI::Spacing();

	glm::vec4 diffuseColor = glm::vec4
    (
        lightComponent->getLight().getDiffuseColor().r,
        lightComponent->getLight().getDiffuseColor().g,
        lightComponent->getLight().getDiffuseColor().b,
        lightComponent->getLight().getDiffuseColor().a
    );

	if (lightComponent->getType() != LightType::AMBIENT)
	{
		EditorUI::LabelText("", "Diffuse Color");
		EditorUI::ColorPicker4("##DiffuseColor", (float*)&diffuseColor, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel);
		for (int i = 1; i <= 2; i++) EditorUI::Spacing();

		glm::vec4 specularColor = glm::vec4
		(
			lightComponent->getLight().getSpecularColor().r,
			lightComponent->getLight().getSpecularColor().g,
			lightComponent->getLight().getSpecularColor().b,
			lightComponent->getLight().getSpecularColor().a
		);

		EditorUI::LabelText("", "Specular Color");
		EditorUI::ColorPicker4("##SpecularColor", (float*)&specularColor, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel);
		for (int i = 1; i <= 2; i++) EditorUI::Spacing();

		float specularShining = lightComponent->getSpecularShining();
		drawInputWidget("##specularShining", "Specular Factor", &specularShining, EditorUI::GetContentRegionAvail().x);

		for (int i = 1; i <= 2; i++) EditorUI::Spacing();

		lightComponent->getLight().setDiffuseColor(ofFloatColor(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a));
		lightComponent->getLight().setSpecularColor(ofFloatColor(specularColor.r, specularColor.g, specularColor.b, specularColor.a));
		lightComponent->setSpecularShining(max(1.0f, specularShining));
	}

	if (lightComponent->getType() == LightType::POINT || lightComponent->getType() == LightType::SPOT)
	{
		float constantAttenuation = lightComponent->getLight().getAttenuationConstant();
		drawInputWidget("##ConstantAttenuation", "Constant Attenuation", &constantAttenuation, EditorUI::GetContentRegionAvail().x);

		float linearAttenuation = lightComponent->getLight().getAttenuationLinear();
		drawInputWidget("##linearAttenuation", "Linear Attenuation", &linearAttenuation, EditorUI::GetContentRegionAvail().x);

		float quadraticAttenuation = lightComponent->getLight().getAttenuationQuadratic();
		drawInputWidget("##quadraticAttenuation", "Quadratic Attenuation", &quadraticAttenuation, EditorUI::GetContentRegionAvail().x);

		lightComponent->getLight().setAttenuation(max(1.0f, constantAttenuation), max(0.0f, linearAttenuation), max(0.0f, quadraticAttenuation));

		if (lightComponent->getType() == LightType::SPOT)
		{
			float attenuationPower = lightComponent->getLight().getSpotConcentration();
			drawInputWidget("##attenuationPower", "Attenuation Power", &attenuationPower, EditorUI::GetContentRegionAvail().x);
			lightComponent->getLight().setSpotConcentration(max(0.0f, attenuationPower));
		}

		for (int i = 1; i <= 2; i++) EditorUI::Spacing();
	}
	
	if (lightComponent->getType() != LightType::AMBIENT)
	{
		EditorUI::Separator();
		for (int i = 1; i <= 4; i++) EditorUI::Spacing();
		EditorUI::Checkbox("Draw Light", lightComponent->showInScene());
		for (int i = 1; i <= 2; i++) EditorUI::Spacing();

		if (*lightComponent->showInScene())
		{
			glm::vec4 editorColor = glm::vec4
			(
				lightComponent->getColor().r,
				lightComponent->getColor().g,
				lightComponent->getColor().b,
				lightComponent->getColor().a
			);

			EditorUI::LabelText("", "Editor Color");
			EditorUI::ColorPicker4("##EditorColor", (float*)&editorColor, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel);
		
			lightComponent->setColor(ofFloatColor(editorColor.r, editorColor.g, editorColor.b, editorColor.a));
		}

		for (int i = 1; i <= 2; i++) EditorUI::Spacing();
	}

	lightComponent->getLight().setAmbientColor(ofFloatColor(ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a));
}

void DrawableComponentPropertiesPanel::drawSectionControlPoints(BezierSurfaceComponent *surfaceComponent)
{
    EditorUI::LabelText("", "Control points");
    vector<vector<glm::vec3>> controlPoints = surfaceComponent->getControlPoints();

    EditorUI::AlignTextToFramePadding();
    EditorUI::Text("Point 1");
    EditorUI::SameLine(0, 32);
    drawInputWidget("##P1X", "X", &(controlPoints[0][0].x), EditorUI::GetContentRegionAvail().x / 3.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P1Y", "Y", &(controlPoints[0][0].y), EditorUI::GetContentRegionAvail().x / 2.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P1Z", "Z", &(controlPoints[0][0].z), EditorUI::GetContentRegionAvail().x, 0.5f);

    EditorUI::AlignTextToFramePadding();
    EditorUI::Text("Point 2");
    EditorUI::SameLine(0, 32);
    drawInputWidget("##P2X", "X", &(controlPoints[0][1].x), EditorUI::GetContentRegionAvail().x / 3.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P2Y", "Y", &(controlPoints[0][1].y), EditorUI::GetContentRegionAvail().x / 2.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P2Z", "Z", &(controlPoints[0][1].z), EditorUI::GetContentRegionAvail().x, 0.5f);

    EditorUI::AlignTextToFramePadding();
    EditorUI::Text("Point 3");
    EditorUI::SameLine(0, 32);
    drawInputWidget("##P3X", "X", &(controlPoints[0][2].x), EditorUI::GetContentRegionAvail().x / 3.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P3Y", "Y", &(controlPoints[0][2].y), EditorUI::GetContentRegionAvail().x / 2.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P3Z", "Z", &(controlPoints[0][2].z), EditorUI::GetContentRegionAvail().x, 0.5f);

    EditorUI::AlignTextToFramePadding();
    EditorUI::Text("Point 4");
    EditorUI::SameLine(0, 32);
    drawInputWidget("##P4X", "X", &(controlPoints[0][3].x), EditorUI::GetContentRegionAvail().x / 3.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P4Y", "Y", &(controlPoints[0][3].y), EditorUI::GetContentRegionAvail().x / 2.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P4Z", "Z", &(controlPoints[0][3].z), EditorUI::GetContentRegionAvail().x, 0.5f);

    EditorUI::AlignTextToFramePadding();
    EditorUI::Text("Point 5");
    EditorUI::SameLine(0, 32);
    drawInputWidget("##P5X", "X", &(controlPoints[1][0].x), EditorUI::GetContentRegionAvail().x / 3.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P5Y", "Y", &(controlPoints[1][0].y), EditorUI::GetContentRegionAvail().x / 2.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P5Z", "Z", &(controlPoints[1][0].z), EditorUI::GetContentRegionAvail().x, 0.5f);

    EditorUI::AlignTextToFramePadding();
    EditorUI::Text("Point 6");
    EditorUI::SameLine(0, 32);
    drawInputWidget("##P6X", "X", &(controlPoints[1][1].x), EditorUI::GetContentRegionAvail().x / 3.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P6Y", "Y", &(controlPoints[1][1].y), EditorUI::GetContentRegionAvail().x / 2.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P6Z", "Z", &(controlPoints[1][1].z), EditorUI::GetContentRegionAvail().x, 0.5f);

    EditorUI::AlignTextToFramePadding();
    EditorUI::Text("Point 7");
    EditorUI::SameLine(0, 32);
    drawInputWidget("##P7X", "X", &(controlPoints[1][2].x), EditorUI::GetContentRegionAvail().x / 3.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P7Y", "Y", &(controlPoints[1][2].y), EditorUI::GetContentRegionAvail().x / 2.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P7Z", "Z", &(controlPoints[1][2].z), EditorUI::GetContentRegionAvail().x, 0.5f);

    EditorUI::AlignTextToFramePadding();
    EditorUI::Text("Point 8");
    EditorUI::SameLine(0, 32);
    drawInputWidget("##P8X", "X", &(controlPoints[1][3].x), EditorUI::GetContentRegionAvail().x / 3.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P8Y", "Y", &(controlPoints[1][3].y), EditorUI::GetContentRegionAvail().x / 2.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P8Z", "Z", &(controlPoints[1][3].z), EditorUI::GetContentRegionAvail().x, 0.5f);

    EditorUI::AlignTextToFramePadding();
    EditorUI::Text("Point 9");
    EditorUI::SameLine(0, 32);
    drawInputWidget("##P9X", "X", &(controlPoints[2][0].x), EditorUI::GetContentRegionAvail().x / 3.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P9Y", "Y", &(controlPoints[2][0].y), EditorUI::GetContentRegionAvail().x / 2.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P9Z", "Z", &(controlPoints[2][0].z), EditorUI::GetContentRegionAvail().x, 0.5f);

    EditorUI::AlignTextToFramePadding();
    EditorUI::Text("Point 10");
    EditorUI::SameLine(0, 32);
    drawInputWidget("##P10X", "X", &(controlPoints[2][1].x), EditorUI::GetContentRegionAvail().x / 3.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P10Y", "Y", &(controlPoints[2][1].y), EditorUI::GetContentRegionAvail().x / 2.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P10Z", "Z", &(controlPoints[2][1].z), EditorUI::GetContentRegionAvail().x, 0.5f);

    EditorUI::AlignTextToFramePadding();
    EditorUI::Text("Point 11");
    EditorUI::SameLine(0, 32);
    drawInputWidget("##P11X", "X", &(controlPoints[2][2].x), EditorUI::GetContentRegionAvail().x / 3.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P11Y", "Y", &(controlPoints[2][2].y), EditorUI::GetContentRegionAvail().x / 2.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P11Z", "Z", &(controlPoints[2][2].z), EditorUI::GetContentRegionAvail().x, 0.5f);

    EditorUI::AlignTextToFramePadding();
    EditorUI::Text("Point 12");
    EditorUI::SameLine(0, 32);
    drawInputWidget("##P12X", "X", &(controlPoints[2][3].x), EditorUI::GetContentRegionAvail().x / 3.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P12Y", "Y", &(controlPoints[2][3].y), EditorUI::GetContentRegionAvail().x / 2.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P12Z", "Z", &(controlPoints[2][3].z), EditorUI::GetContentRegionAvail().x, 0.5f);

    EditorUI::AlignTextToFramePadding();
    EditorUI::Text("Point 13");
    EditorUI::SameLine(0, 32);
    drawInputWidget("##P13X", "X", &(controlPoints[3][0].x), EditorUI::GetContentRegionAvail().x / 3.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P13Y", "Y", &(controlPoints[3][0].y), EditorUI::GetContentRegionAvail().x / 2.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P13Z", "Z", &(controlPoints[3][0].z), EditorUI::GetContentRegionAvail().x, 0.5f);

    EditorUI::AlignTextToFramePadding();
    EditorUI::Text("Point 14");
    EditorUI::SameLine(0, 32);
    drawInputWidget("##P14X", "X", &(controlPoints[3][1].x), EditorUI::GetContentRegionAvail().x / 3.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P14Y", "Y", &(controlPoints[3][1].y), EditorUI::GetContentRegionAvail().x / 2.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P14Z", "Z", &(controlPoints[3][1].z), EditorUI::GetContentRegionAvail().x, 0.5f);

    EditorUI::AlignTextToFramePadding();
    EditorUI::Text("Point 15");
    EditorUI::SameLine(0, 32);
    drawInputWidget("##P15X", "X", &(controlPoints[3][2].x), EditorUI::GetContentRegionAvail().x / 3.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P15Y", "Y", &(controlPoints[3][2].y), EditorUI::GetContentRegionAvail().x / 2.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P15Z", "Z", &(controlPoints[3][2].z), EditorUI::GetContentRegionAvail().x, 0.5f);

    EditorUI::AlignTextToFramePadding();
    EditorUI::Text("Point 16");
    EditorUI::SameLine(0, 32);
    drawInputWidget("##P16X", "X", &(controlPoints[3][3].x), EditorUI::GetContentRegionAvail().x / 3.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P16Y", "Y", &(controlPoints[3][3].y), EditorUI::GetContentRegionAvail().x / 2.0f, 0.5f);
    EditorUI::SameLine();
    drawInputWidget("##P16Z", "Z", &(controlPoints[3][3].z), EditorUI::GetContentRegionAvail().x, 0.5f);

    surfaceComponent->setControlPoints(controlPoints);
}

void DrawableComponentPropertiesPanel::addViewport(AbstractViewport *viewport)
{
    viewportNames.push_back(viewport->getName());
    viewports.insert(std::pair<string, AbstractViewport *>(viewport->getName(), std::move(viewport)));
}

void DrawableComponentPropertiesPanel::removeViewport(AbstractViewport *viewport)
{
    if (strcmp(viewport->getName().data(), selectedViewportName) == 0)
    {
        selectedViewportName = TYPE_NONE;
        selectedViewport = viewports.at(TYPE_NONE);
    }

    auto iteratorViewportNames = find(viewportNames.begin(), viewportNames.end(), viewport->getName());
    if (iteratorViewportNames != viewportNames.end())
        viewportNames.erase(iteratorViewportNames);

    auto iteratorViewports = viewports.find(viewport->getName());
    if (iteratorViewports != viewports.end())
        viewports.erase(iteratorViewports);

    for (Entity *entity : scene->getEntityTree()->getRootEntities())
    {
        Shape3dComponent *shape3dComponent = dynamic_cast<Shape3dComponent *>(entity->getDrawableComponent());
        if (shape3dComponent == nullptr)
            continue;

        RenderTextureComponent *renderTextureComponent = dynamic_cast<RenderTextureComponent *>(shape3dComponent->getTextureComponent());
        if (renderTextureComponent != nullptr && renderTextureComponent->getSource() == viewport)
            renderTextureComponent->setSource(nullptr);
    }
}

void DrawableComponentPropertiesPanel::drawSectionModel(ModelComponent *modelComponent)
{
    Action menuAction = Action::NONE;

    if (!modelComponent->getModelLoaded())
    {
        EditorUI::SetNextItemWidth(400);
        if (EditorUI::Button("Load custom model", {200.0f, 22.0f}))
        {
            loadModelFromFile(menuAction, modelComponent, "");
        }
        EditorUI::SetNextItemWidth(400);
        if (EditorUI::Button("Load tea pot model", {200.0f, 22.0f}))
        {
            loadModelFromFile(menuAction, modelComponent, "resources/models/pot/pot.fbx");
        }
        EditorUI::SetNextItemWidth(400);
        if (EditorUI::Button("Load animated character model", {200.0f, 22.0f}))
        {
            loadModelFromFile(menuAction, modelComponent, "resources/models/zombie/zombie.fbx");
        }
        EditorUI::SetNextItemWidth(400);
        if (EditorUI::Button("Load jet model", {200.0f, 22.0f}))
        {
            loadModelFromFile(menuAction, modelComponent, "resources/models/plane/plane.fbx");
        }

        if (menuAction == Action::IMPORT_MODEL &&
            !modelComponent->getModelLoaded())
        {
            EditorUI::OpenPopup("Error");
        }

        if (EditorUI::BeginPopupModal("Error", nullptr, ImGuiWindowFlags_NoResize))
        {
            EditorUI::Text(Interface::getErrorMessage(Action::IMPORT_MODEL).c_str());

            EditorUI::Spacing();
            EditorUI::SameLine(375);
            if (EditorUI::Button("OK", ImVec2(120, 0)))
                EditorUI::CloseCurrentPopup();

            EditorUI::EndPopup();
        }
    }
    else
    {
        EditorUI::Text(modelComponent->getModelName().c_str());
        EditorUI::SameLine();
        if (EditorUI::Button("Clear Model"))
        {
            modelComponent->unloadModel();
        }
        if (modelComponent->hasAnimation())
        {
            const int animationCount = modelComponent->getAnimationCount();
            if (EditorUI::BeginCombo("Animation list", std::to_string(selectedAnimation).c_str()))
            {
                for (int i = 0; i < modelComponent->getAnimationCount(); i++)
                {
                    bool isSelected = (selectedAnimation == i);
                    if (EditorUI::Selectable(std::to_string(i).c_str(), isSelected))
                    {
                        modelComponent->pauseAnimation(selectedAnimation);
                        selectedAnimation = i;
                    }
                    if (isSelected)
                        EditorUI::SetItemDefaultFocus();
                }
                EditorUI::EndCombo();
            }
            if (EditorUI::Button(modelComponent->getAnimationPlaying() ? "Pause" : "Play"))
            {
                if (modelComponent->getAnimationPlaying())
                    modelComponent->pauseAnimation(selectedAnimation);
                else
                    modelComponent->playAnimation(selectedAnimation);
            }
        }

        drawSectionRenderMode();
        drawSectionTexture();
    }
}

void DrawableComponentPropertiesPanel::drawSectionMetallicBall(MetallicBallComponent* metallicBallComponent)
{
    roughness = metallicBallComponent->getRoughness();
    normalValue = metallicBallComponent->getNormalValue();

    EditorUI::LabelText("", "Roughness");
    EditorUI::DragFloat("##DragFloatRoughness", &roughness, 0.001f, 0.0f, 1.0f);

    EditorUI::LabelText("", "Normal Value");
    EditorUI::DragFloat("##DragFloatNormalValue", &normalValue, 0.001f, 0.0f, 1.0f);

    metallicBallComponent->setRoughness(roughness);
    metallicBallComponent->setNormalValue(normalValue);
}

void DrawableComponentPropertiesPanel::drawSectionColor()
{
    color = glm::vec4(
        Interface::getSelectedEntity()->getDrawableComponent()->getColor().r,
        Interface::getSelectedEntity()->getDrawableComponent()->getColor().g,
        Interface::getSelectedEntity()->getDrawableComponent()->getColor().b,
        Interface::getSelectedEntity()->getDrawableComponent()->getColor().a);

    EditorUI::LabelText("", "Color");
    EditorUI::ColorPicker4("Color", (float *)&color, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel);

    Interface::getSelectedEntity()->getDrawableComponent()->setColor(ofFloatColor(color.r, color.g, color.b, color.a));
}

void DrawableComponentPropertiesPanel::drawSectionRenderMode()
{
    int renderMode = static_cast<int>(Interface::getSelectedEntity()->getDrawableComponent()->getRenderMode());

    EditorUI::Separator();

    EditorUI::LabelText("", "Render mode");
    EditorUI::RadioButton("Solid", &renderMode, static_cast<int>(ofPolyRenderMode::OF_MESH_FILL));
    EditorUI::RadioButton("Wireframe", &renderMode, static_cast<int>(ofPolyRenderMode::OF_MESH_WIREFRAME));
    EditorUI::RadioButton("Vertex", &renderMode, static_cast<int>(ofPolyRenderMode::OF_MESH_POINTS));

    Interface::getSelectedEntity()->getDrawableComponent()->setRenderMode((ofPolyRenderMode)renderMode);
}

void DrawableComponentPropertiesPanel::drawSectionTexture()
{
    Shape3dComponent *shape3dComponent = dynamic_cast<Shape3dComponent *>(Interface::getSelectedEntity()->getDrawableComponent());
    if (shape3dComponent == nullptr)
        return;

    TextureComponent *textureComponent = dynamic_cast<TextureComponent *>(shape3dComponent->getTextureComponent());
    selectedTextureType = (textureComponent != nullptr ? static_cast<int>(textureComponent->getType()) : static_cast<int>(TextureTypesEnum::NONE)) + 1;

    EditorUI::Separator();

    EditorUI::LabelText("", "Texture");
    ImGui::Combo("##ComboTextures", &selectedTextureType, textureTypes, IM_ARRAYSIZE(textureTypes));

    //No texture
    if (strcmp(TYPE_NONE, textureTypes[selectedTextureType]) == 0)
    {
        shape3dComponent->setTextureComponent(nullptr);
    }

    //Gradient texture
    else if (strcmp(TYPE_TEXTURE_GRADIENT, textureTypes[selectedTextureType]) == 0)
    {
        GradientTextureComponent *gradientTextureComponent = dynamic_cast<GradientTextureComponent *>(textureComponent);
        textureComponent = gradientTextureComponent;

        if (gradientTextureComponent != nullptr)
        {
            selectedGradientType = static_cast<int>(gradientTextureComponent->getType());
            gradientBeginColor = glm::vec4(
                gradientTextureComponent->getBeginColor().r,
                gradientTextureComponent->getBeginColor().g,
                gradientTextureComponent->getBeginColor().b,
                gradientTextureComponent->getBeginColor().a);
            gradientEndColor = glm::vec4(
                gradientTextureComponent->getEndColor().r,
                gradientTextureComponent->getEndColor().g,
                gradientTextureComponent->getEndColor().b,
                gradientTextureComponent->getEndColor().a);
        }

        EditorUI::LabelText("", "Gradient type");
        EditorUI::RadioButton("Horizontal", &selectedGradientType, static_cast<int>(GradientTypesEnum::HORIZONTAL));
        EditorUI::RadioButton("Vertical", &selectedGradientType, static_cast<int>(GradientTypesEnum::VERTICAL));

        EditorUI::LabelText("", "Begin color");
        EditorUI::ColorPicker4("Begin color", (float *)&gradientBeginColor, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel);

        EditorUI::LabelText("", "End color");
        EditorUI::ColorPicker4("End color", (float *)&gradientEndColor, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel);

        if (gradientTextureComponent == nullptr)
        {
            gradientTextureComponent = new GradientTextureComponent();
            shape3dComponent->setTextureComponent(gradientTextureComponent);
        }

        gradientTextureComponent->setType(static_cast<GradientTypesEnum>(selectedGradientType));
        gradientTextureComponent->setBeginColor(ofFloatColor(gradientBeginColor.x, gradientBeginColor.y, gradientBeginColor.z, gradientBeginColor.w));
        gradientTextureComponent->setEndColor(ofFloatColor(gradientEndColor.x, gradientEndColor.y, gradientEndColor.z, gradientEndColor.w));
        shape3dComponent->mapTexCoordsFromTexture();
    }

    //Image texture
    else if (strcmp(TYPE_TEXTURE_IMAGE, textureTypes[selectedTextureType]) == 0)
    {
        ImageTextureComponent *imageTextureComponent = dynamic_cast<ImageTextureComponent *>(textureComponent);
        textureComponent = imageTextureComponent;

        if (imageTextureComponent != nullptr)
        {
            strcpy(selectedImagePath, imageTextureComponent->getImagePath().c_str());
            selectedImage = imageTextureComponent->getImage();
        }

        EditorUI::LabelText("", "Image Path");

        if (EditorUI::Button("Choose"))
        {
            Action menuAction = Action::NONE;
            if (loadImageFromFile(menuAction))
            {
                imageTextureComponent->setImagePath(selectedImagePath);
                imageTextureComponent->setImage(selectedImage);
                imageTextureComponent->update(true);
                shape3dComponent->mapTexCoordsFromTexture();
            }
        }
        EditorUI::SameLine();
        EditorUI::LabelText("", selectedImagePath);

        if (imageTextureComponent == nullptr)
        {
            imageTextureComponent = new ImageTextureComponent();
            shape3dComponent->setTextureComponent(imageTextureComponent);
        }
    }
}

void DrawableComponentPropertiesPanel::drawSectionFilter()
{
    Shape3dComponent *shape3dComponent = dynamic_cast<Shape3dComponent *>(Interface::getSelectedEntity()->getDrawableComponent());
    if (shape3dComponent == nullptr)
        return;

    TextureComponent *textureComponent = dynamic_cast<TextureComponent *>(shape3dComponent->getTextureComponent());
    if (textureComponent == nullptr)
        return;

    FilterComponent *filterComponent = dynamic_cast<FilterComponent *>(shape3dComponent->getFilterComponent());

    selectedFilterType = (filterComponent != nullptr ? static_cast<int>(filterComponent->getType()) : static_cast<int>(FilterTypesEnum::NONE)) + 1;

    EditorUI::Separator();

    EditorUI::LabelText("", "Filter");
    ImGui::Combo("##ComboFilters", &selectedFilterType, filterTypes, IM_ARRAYSIZE(filterTypes));

    if (strcmp(TYPE_NONE, filterTypes[selectedFilterType]) == 0)
    {
        shape3dComponent->setFilterComponent(nullptr);
    }
    else if (strcmp(TYPE_FILTER_GRAYSCALE, filterTypes[selectedFilterType]) == 0)
    {
        GrayscaleFilterComponent *grayscaleFilterComponent = dynamic_cast<GrayscaleFilterComponent *>(filterComponent);

        if (grayscaleFilterComponent == nullptr)
        {
            grayscaleFilterComponent = new GrayscaleFilterComponent();
            shape3dComponent->setFilterComponent(grayscaleFilterComponent);
        }
    }
    else if (strcmp(TYPE_FILTER_SEPIA, filterTypes[selectedFilterType]) == 0)
    {
        SepiaFilterComponent *sepiaFilterComponent = dynamic_cast<SepiaFilterComponent *>(filterComponent);

        if (sepiaFilterComponent == nullptr)
        {
            sepiaFilterComponent = new SepiaFilterComponent();
            shape3dComponent->setFilterComponent(sepiaFilterComponent);
        }
    }
    else if (strcmp(TYPE_FILTER_INVERT, filterTypes[selectedFilterType]) == 0)
    {
        InvertFilterComponent *invertFilterComponent = dynamic_cast<InvertFilterComponent *>(filterComponent);

        if (invertFilterComponent == nullptr)
        {
            invertFilterComponent = new InvertFilterComponent();
            shape3dComponent->setFilterComponent(invertFilterComponent);
        }
    }
}

void DrawableComponentPropertiesPanel::loadModelFromFile(Action &menuAction, ModelComponent *modelComponent, string modelPath)
{
    menuAction = Action::IMPORT_MODEL;
    ModelImport modelImport;
    ofxAssimpModelLoader *model = new ofxAssimpModelLoader();
    modelImport.modelPtr = model;
    modelImport.modelName = modelPath;
    if (Interface::executeCommand(menuAction, &modelImport))
    {
        modelComponent->setModelName(modelImport.modelName);
        modelComponent->setModel(model);
        model->setLoopStateForAllAnimations(OF_LOOP_NORMAL);
        selectedAnimation = 0;
    }
}

bool DrawableComponentPropertiesPanel::loadImageFromFile(Action &menuAction)
{
    menuAction = Action::IMPORT_IMAGE;
    ImageImport imageImport;
    ofImage *image = new ofImage();
    imageImport.imagePtr = image;

    if (Interface::executeCommand(menuAction, &imageImport))
    {
        strcpy(selectedImagePath, imageImport.imagePath.c_str());
        selectedImage = imageImport.imagePtr;

        return true;
    }

    delete image;
    return false;
}
