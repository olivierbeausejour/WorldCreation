#pragma once

#include "wcpch.h"

#include "drawableComponent.h"

#include "helpers/mathHelper.h"


class Shape2dComponent : public DrawableComponent
{

public:

	Shape2dComponent() : DrawableComponent() { }
	~Shape2dComponent() { }

	ofFloatColor getOutlineColor() const { return outlineColor; }
	void setOutlineColor(const ofFloatColor color) { outlineColor = color; }

	int getLineWidth() const { return lineWidth; }
	void setLineWidth(const int width) { lineWidth = width; }

	void draw() override;

protected:

	void setSize(const glm::vec3 size) override;

	vector<ofVec2f> vertices = vector<ofVec2f>();
	ofFloatColor outlineColor;
	int lineWidth = 1;

private:

	static constexpr float MAX_INTERSECTION_DISTANCE_CHECK = 100000.0f;

};