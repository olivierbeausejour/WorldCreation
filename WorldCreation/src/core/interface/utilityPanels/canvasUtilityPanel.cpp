#include "canvasUtilityPanel.h"

void CanvasUtilityPanel::draw()
{
	EditorUI::Begin("Canvas Tools", NULL);

    if (EditorUI::CollapsingHeader("Tools", ImGuiTreeNodeFlags_DefaultOpen))
    {
        EditorUI::LabelText("", "Primitives");
        EditorUI::RadioButton("Line", &selectedTool, static_cast<int>(Shape2dTypesEnum::LINE));
        EditorUI::RadioButton("Rectangle", &selectedTool, static_cast<int>(Shape2dTypesEnum::RECTANGLE));
        EditorUI::RadioButton("Square", &selectedTool, static_cast<int>(Shape2dTypesEnum::SQUARE));
        EditorUI::RadioButton("Ellipse", &selectedTool, static_cast<int>(Shape2dTypesEnum::ELLIPSE));
        EditorUI::RadioButton("Circle", &selectedTool, static_cast<int>(Shape2dTypesEnum::CIRCLE));
        EditorUI::RadioButton("Triangle", &selectedTool, static_cast<int>(Shape2dTypesEnum::TRIANGLE));
        EditorUI::RadioButton("Right Triangle", &selectedTool, static_cast<int>(Shape2dTypesEnum::RIGHT_TRIANGLE));
        EditorUI::RadioButton("Bezier Curve", &selectedTool, static_cast<int>(Shape2dTypesEnum::BEZIER_CURVE));
        
        EditorUI::Separator();
        
        EditorUI::LabelText("", "Shapes");
        EditorUI::RadioButton("Flower", &selectedTool, static_cast<int>(Shape2dTypesEnum::FLOWER));
        EditorUI::RadioButton("Horizontal Arrow", &selectedTool, static_cast<int>(Shape2dTypesEnum::HORIZONTAL_ARROW));
        EditorUI::RadioButton("Vertical Arrow", &selectedTool, static_cast<int>(Shape2dTypesEnum::VERTICAL_ARROW));

        EditorUI::LabelText("", "Properties");
        EditorUI::DragInt("Line Width", &lineWidth, 1, 1, 100);

        if (updateHistogram)
        {
            EditorUI::OpenPopup("Canvas Histogram");
            updateHistogram = false;
        }

        EditorUI::Separator();

        for (int i = 1; i <= 4; i++) EditorUI::Spacing();

        if (EditorUI::Button("Show Histogram", ImVec2(EditorUI::GetContentRegionAvail().x, 0)))
            updateHistogram = true;

        if (EditorUI::BeginPopupModal("Canvas Histogram", nullptr, ImGuiWindowFlags_NoResize))
        {
            // Red Histogram
            EditorUI::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            EditorUI::PushStyleColor(ImGuiCol_PlotHistogramHovered, (ImVec4)ImColor::HSV(0, 1.0f, 0.6f));
            EditorUI::PlotHistogram("", redPixelsOccurrence.data(), 256, 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(400, 150));
            EditorUI::PopStyleColor(2);

            // Green Histogram
            EditorUI::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
            EditorUI::PushStyleColor(ImGuiCol_PlotHistogramHovered, (ImVec4)ImColor::HSV(0.375f, 1.0f, 0.6f));
            EditorUI::PlotHistogram("", greenPixelsOccurrence.data(), 256, 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(400, 150));
            EditorUI::PopStyleColor(2);

            // Blue Histogram
            EditorUI::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
            EditorUI::PushStyleColor(ImGuiCol_PlotHistogramHovered, (ImVec4)ImColor::HSV(0.65f, 1.0f, 0.6f));
            EditorUI::PlotHistogram("", bluePixelsOccurrence.data(), 256, 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(400, 150));
            EditorUI::PopStyleColor(2);

            for (int i = 1; i <= 3; i++) EditorUI::Spacing();

            EditorUI::SameLine(EditorUI::GetContentRegionAvail().x - 112);
            if (EditorUI::Button("OK", ImVec2(120, 0)))
                EditorUI::CloseCurrentPopup();
          
            EditorUI::EndPopup();
        }
        EditorUI::Dummy(ImVec2(0, 10));
    }

    if (EditorUI::CollapsingHeader("Colors", ImGuiTreeNodeFlags_DefaultOpen))
    {
        EditorUI::LabelText("", "Fill Color");
        EditorUI::ColorPicker4("Fill Color", (float*)&fillColor, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel);

        EditorUI::Separator();

        EditorUI::LabelText("", "Outline Color");
        EditorUI::ColorPicker4("Outline Color", (float*)&outlineColor, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel);

        EditorUI::Separator();

        EditorUI::LabelText("", "Background Color");
        EditorUI::ColorPicker4("Background Color", (float*)&backgroundColor, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel);
    }

	EditorUI::End();
}
