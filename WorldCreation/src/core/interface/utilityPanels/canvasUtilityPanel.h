#pragma once

#include "wcpch.h"

#include "entity/components/shape2dTypesEnum.h"


class CanvasUtilityPanel
{

public:

	CanvasUtilityPanel(): redPixelsOccurrence(256, 0), greenPixelsOccurrence(256, 0), bluePixelsOccurrence(256, 0) { };
	~CanvasUtilityPanel() { };

	Shape2dTypesEnum getSelectedTool() { return static_cast<Shape2dTypesEnum>(selectedTool); }
	int getLineWidth() { return lineWidth; }
	ofFloatColor getFillColor() { return ofFloatColor(fillColor.x, fillColor.y, fillColor.z, fillColor.w); }
	ofFloatColor getOutlineColor() { return ofFloatColor(outlineColor.x, outlineColor.y, outlineColor.z, outlineColor.w); }
	ofFloatColor getBackgroundColor() { return ofFloatColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w); }

	
	void setRedPixelOccurrence(const size_t index, const float occurrence) { redPixelsOccurrence[index] = occurrence; }
	float getRedPixelOccurrence(const size_t index) const { return redPixelsOccurrence[index]; }

	void setGreenPixelOccurrence(const size_t index, const float occurrence) { greenPixelsOccurrence[index] = occurrence; }
	float getGreenPixelOccurrence(const size_t index) const { return greenPixelsOccurrence[index]; }

	void setBluePixelOccurrence(const size_t index, const float occurrence) { bluePixelsOccurrence[index] = occurrence; }
	float getBluePixelOccurrence(const size_t index) const { return bluePixelsOccurrence[index]; }

	void setUpdateHistogram(const bool update) { updateHistogram = update; }
	bool getUpdateHistogram() const { return updateHistogram; }

	void draw();

private:
	bool updateHistogram = false;
	int selectedTool = static_cast<int>(Shape2dTypesEnum::LINE);
	int lineWidth = 1;
	ImVec4 fillColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	ImVec4 outlineColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	ImVec4 backgroundColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

	std::vector<float> redPixelsOccurrence;
	std::vector<float> greenPixelsOccurrence;
	std::vector<float> bluePixelsOccurrence;
};