#include "entityPropertiesPanel.h"
#include "interface/interface.h"

void EntityPropertiesPanel::setup(Scene *scene)
{
	this->scene = scene;
	drawablePanel->setup(scene);
}

void EntityPropertiesPanel::draw()
{
	EditorUI::Begin("Properties Panel", NULL);
	if (Interface::getSelectedEntity() != nullptr)
	{
		transformPanel->draw();
		drawablePanel->draw();
		materialPanel->draw();

		for (int i = 1; i <= 3; i++)
			EditorUI::Spacing();
		ImGui::Separator();
		for (int i = 1; i <= 3; i++)
			EditorUI::Spacing();

		AddPopupContext(Interface::getSelectedEntity());
	}
	EditorUI::End();
}

void EntityPropertiesPanel::AddPopupContext(Entity *currentEntity)
{
	if (ImGui::Button("Select component"))
	{
		ImGui::OpenPopup("select_component_popup");
	}
	ImGui::SameLine();
	if (ImGui::BeginPopup("select_component_popup"))
	{
		if (ImGui::BeginMenu("3D component"))
		{
			if (EditorUI::MenuItem("None"))
			{
				Interface::getSelectedEntity()->setDrawableComponent(nullptr);
			}
			if (EditorUI::MenuItem("Cube"))
			{
				Interface::getSelectedEntity()->setDrawableComponent(new CubeComponent());
			}
			if (EditorUI::MenuItem("Cone"))
			{
				Interface::getSelectedEntity()->setDrawableComponent(new ConeComponent());
			}
			if (EditorUI::MenuItem("Plane"))
			{
				Interface::getSelectedEntity()->setDrawableComponent(new PlaneComponent());
			}
			if (EditorUI::MenuItem("Sphere"))
			{
				Interface::getSelectedEntity()->setDrawableComponent(new SphereComponent());
			}
			if (EditorUI::MenuItem("Surface"))
			{
				Interface::getSelectedEntity()->setDrawableComponent(new BezierSurfaceComponent());
			}
			if (EditorUI::MenuItem("Metallic Ball"))
			{
				Interface::getSelectedEntity()->setDrawableComponent(new MetallicBallComponent());
			}
			if (EditorUI::MenuItem("Model"))
			{
				Interface::getSelectedEntity()->setDrawableComponent(new ModelComponent());
			}
			ImGui::EndMenu();
		}
		
		if (scene->getLightEntities().size() < LightComponent::MAX_NUMBER_OF_LIGHTS && ImGui::BeginMenu("Light"))
		{
			if (EditorUI::MenuItem("Ambient"))
			{
				Interface::getSelectedEntity()->setDrawableComponent(new LightComponent(LightType::AMBIENT));
			}
			if (EditorUI::MenuItem("Directional"))
			{
				Interface::getSelectedEntity()->setDrawableComponent(new LightComponent(LightType::DIRECTIONAL));
			}
			if (EditorUI::MenuItem("Point Light"))
			{
				Interface::getSelectedEntity()->setDrawableComponent(new LightComponent(LightType::POINT));
			}
			if (EditorUI::MenuItem("Spotlight"))
			{
				Interface::getSelectedEntity()->setDrawableComponent(new LightComponent(LightType::SPOT));
			}
			ImGui::EndMenu();
		}
		
		ImGui::EndPopup();
	}
}
