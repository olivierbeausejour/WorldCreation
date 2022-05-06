#pragma once

#include "wcpch.h"

#include "shape2dComponent.h"
#include "lineDrawer.hpp"

class Entity;

class BezierCurveComponent : public Shape2dComponent
{

public:
    BezierCurveComponent();
    ~BezierCurveComponent() {}

	void addControlPoint(float x, float y);
	void updateControlPoint(int index, float x, float y) { controlPoints[index] = glm::vec2(x, y); updateCurve(); };
	int getSelectedPoint(float mouseX, float mouseY);

    void draw() override;

private:
	const float CONTROL_POINT_RADIUS = 10;
    vector<glm::vec2> controlPoints;
    vector<Entity> curveLines;
    LineDrawer lineDrawer;

    glm::vec2 interpolateBezier(float t);
    void updateCurve();
};