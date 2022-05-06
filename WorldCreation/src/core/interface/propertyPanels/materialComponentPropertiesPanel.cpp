#include "materialComponentPropertiesPanel.h"
#include "interface/interface.h"

void MaterialComponentPropertiesPanel::draw()
{
	if (Interface::getSelectedEntity() != nullptr && 
        Interface::getSelectedEntity()->getDrawableComponent() != nullptr)
    {
		if (dynamic_cast<LightComponent*>(Interface::getSelectedEntity()->getDrawableComponent()) == nullptr)
		{
			if (dynamic_cast<MetallicBallComponent*>(Interface::getSelectedEntity()->getDrawableComponent()) == nullptr && 
				dynamic_cast<BezierSurfaceComponent*>(Interface::getSelectedEntity()->getDrawableComponent()) == nullptr)
			{
				if (EditorUI::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
					drawMaterialSection(Interface::getSelectedEntity()->getDrawableComponent());
			}
		}
	}
}

void MaterialComponentPropertiesPanel::drawMaterialSection(DrawableComponent* drawableComponent)
{
	glm::vec4 ambientColor = glm::vec4
    (
        drawableComponent->getMaterial().getAmbientColor().r,
        drawableComponent->getMaterial().getAmbientColor().g,
        drawableComponent->getMaterial().getAmbientColor().b,
        drawableComponent->getMaterial().getAmbientColor().a
    );

	EditorUI::LabelText("", "Ambient Color");
	EditorUI::ColorPicker4("##MaterialAmbientColor", (float*)&ambientColor, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel);
	for (int i = 1; i <= 2; i++) EditorUI::Spacing();

	glm::vec4 diffuseColor = glm::vec4
    (
        drawableComponent->getMaterial().getDiffuseColor().r,
        drawableComponent->getMaterial().getDiffuseColor().g,
        drawableComponent->getMaterial().getDiffuseColor().b,
        drawableComponent->getMaterial().getDiffuseColor().a
    );

	EditorUI::LabelText("", "Diffuse Color");
	EditorUI::ColorPicker4("##MaterialDiffuseColor", (float*)&diffuseColor, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel);
	for (int i = 1; i <= 2; i++) EditorUI::Spacing();

	glm::vec4 specularColor = glm::vec4
    (
        drawableComponent->getMaterial().getSpecularColor().r,
        drawableComponent->getMaterial().getSpecularColor().g,
        drawableComponent->getMaterial().getSpecularColor().b,
        drawableComponent->getMaterial().getSpecularColor().a
    );

	EditorUI::LabelText("", "Specular Color");
	EditorUI::ColorPicker4("##MaterialSpecularColor", (float*)&specularColor, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel);
	for (int i = 1; i <= 2; i++) EditorUI::Spacing();

	drawableComponent->getMaterial().setAmbientColor(ofFloatColor(ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a));
	drawableComponent->getMaterial().setDiffuseColor(ofFloatColor(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a));
	drawableComponent->getMaterial().setSpecularColor(ofFloatColor(specularColor.r, specularColor.g, specularColor.b, specularColor.a));
}