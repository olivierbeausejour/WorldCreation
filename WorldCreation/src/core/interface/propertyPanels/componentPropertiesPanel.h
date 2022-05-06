#pragma once

class ComponentPropertiesPanel
{

public:

	virtual void draw() = 0;
	static inline void drawInputWidget(char* id, char* label, float* value, float width, float step=1.0f, float fastStep=2.0f)
	{
		std::string format = std::string(label) + std::string(" : %0.2f");
		EditorUI::AlignTextToFramePadding();
		EditorUI::PushItemWidth(width);
		EditorUI::InputFloat(id, value, step, fastStep, format.c_str());
		EditorUI::PopItemWidth();
	}
};

