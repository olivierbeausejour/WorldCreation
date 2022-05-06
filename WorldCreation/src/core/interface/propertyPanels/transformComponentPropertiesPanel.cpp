#include "transformComponentPropertiesPanel.h"
#include "interface/interface.h"

void TransformComponentPropertiesPanel::draw()
{
    if (Interface::getSelectedEntity() != nullptr &&
        Interface::getSelectedEntity()->getTransform() != nullptr)
    {
        if (EditorUI::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            for (int i = 1; i <= 2; i++) EditorUI::Spacing();

            position = Interface::getSelectedEntity()->getTransform()->getLocalPosition();
            scale = Interface::getSelectedEntity()->getTransform()->getLocalScale();
            rotation = Interface::getSelectedEntity()->getTransform()->getLocalEulerAngles();

            EditorUI::AlignTextToFramePadding();
            EditorUI::Text("Position");
            EditorUI::SameLine(0, 15);

            // Simple visual trick to avoid -0.00
            position.x = MathHelper::isFloatEqual(0.0f, abs(position.x)) ? 0.0f : position.x;
            position.y = MathHelper::isFloatEqual(0.0f, abs(position.y)) ? 0.0f : -position.y;
            position.z = MathHelper::isFloatEqual(0.0f, abs(position.z)) ? 0.0f : position.z;

            if (showAsCartesian)
            {
                drawInputWidget("##PosX", "X", &(position.x), EditorUI::GetContentRegionAvail().x / 3.0f); EditorUI::SameLine();
                drawInputWidget("##PosY", "Y", &(position.y), EditorUI::GetContentRegionAvail().x / 2.0f); EditorUI::SameLine();
                drawInputWidget("##PosZ", "Z", &(position.z), EditorUI::GetContentRegionAvail().x);
            }
            else
            {
                glm::vec3 cylCoords = MathHelper::cartesianToCylindrical(position);

                drawInputWidget("##Radius", "R", &(cylCoords.x), EditorUI::GetContentRegionAvail().x / 3.0f); EditorUI::SameLine();
                drawInputWidget("##ZCyl", "Y", &(cylCoords.y), EditorUI::GetContentRegionAvail().x / 2.0f); EditorUI::SameLine();
                drawInputWidget("##Azimuth","A", &(cylCoords.z), EditorUI::GetContentRegionAvail().x);

                cylCoords.x = std::max(0.0f, cylCoords.x);

                if (cylCoords.z < 0.0f)
                    cylCoords.z += 360.0f;

                position = MathHelper::cylindricalToCartesian(cylCoords);
            }

            position.y = position.y == 0.0f ? 0.0f : -position.y;

            EditorUI::AlignTextToFramePadding();
            EditorUI::Text("Rotation");
            EditorUI::SameLine(0, 12);
            drawInputWidget("##RotX","X", &(rotation.x), EditorUI::GetContentRegionAvail().x / 3.0f); EditorUI::SameLine();
            drawInputWidget("##RotY", "Y", &(rotation.y), EditorUI::GetContentRegionAvail().x / 2.0f); EditorUI::SameLine();
            drawInputWidget("##RotZ", "Z", &(rotation.z), EditorUI::GetContentRegionAvail().x);

            EditorUI::AlignTextToFramePadding();
            EditorUI::Text("Scale");
            EditorUI::SameLine(0, 32);
            drawInputWidget("##ScaleX", "X", &(scale.x), EditorUI::GetContentRegionAvail().x / 3.0f); EditorUI::SameLine();
            drawInputWidget("##ScaleY", "Y", &(scale.y), EditorUI::GetContentRegionAvail().x / 2.0f); EditorUI::SameLine();
            drawInputWidget("##ScaleZ", "Z", &(scale.z), EditorUI::GetContentRegionAvail().x);

            for (int i = 1; i <= 6; i++) EditorUI::Spacing();
            EditorUI::AlignTextToFramePadding();
            EditorUI::Text("Position Mode");
            EditorUI::SameLine(0, EditorUI::GetContentRegionAvail().x / 8.0f);

            EditorUI::RadioButton("Cartesian", &showAsCartesian, 1);
            EditorUI::SameLine(0, EditorUI::GetContentRegionAvail().x / 6.0f);
            EditorUI::RadioButton("Cylindrical", &showAsCartesian, 0);

            for (int i = 1; i <= 3; i++) EditorUI::Spacing();
            if (EditorUI::Button("Reset", ImVec2(EditorUI::GetWindowSize().x, 25)))
            {
                Interface::getSelectedEntity()->getTransform()->reset();
            }
            else
            {
                Interface::getSelectedEntity()->getTransform()->setLocalPosition(position);
                Interface::getSelectedEntity()->getTransform()->setLocalScale(scale);
                Interface::getSelectedEntity()->getTransform()->setLocalEulerAngles(rotation);
            }

            for (int i = 1; i <= 2; i++) EditorUI::Spacing();
        }      
    }
}
