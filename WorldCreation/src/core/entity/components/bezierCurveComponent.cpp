#include "bezierCurveComponent.h"
#include "entity/entity.h"

BezierCurveComponent::BezierCurveComponent() : Shape2dComponent()
{
    controlPoints.push_back(glm::vec2(10, 100));
    controlPoints.push_back(glm::vec2(50, 400));
    controlPoints.push_back(glm::vec2(350, 300));
    controlPoints.push_back(glm::vec2(400, 400));
    controlPoints.push_back(glm::vec2(500, 400));
    controlPoints.push_back(glm::vec2(600, 400));
    controlPoints.push_back(glm::vec2(700, 400));

    updateCurve();
}

void BezierCurveComponent::updateCurve()
{
	vertices.clear();

	float delta = 0.03f;
    for (float t = 0; t <= 1; t += delta)
    {
        vertices.push_back(interpolateBezier(t));
    }
}

// Notes de cours p.218
glm::vec2 BezierCurveComponent::interpolateBezier(float t)
{
    int n = controlPoints.size() - 1;
    float x = 0;
    float y = 0;
    for (int k = 0; k <= n; k++)
    {
        x += (MathHelper::factorial(n) / (MathHelper::factorial(k) * MathHelper::factorial(n - k))) * pow(t, k) * pow(1 - t, n - k) * controlPoints[k].x;
        y += (MathHelper::factorial(n) / (MathHelper::factorial(k) * MathHelper::factorial(n - k))) * pow(t, k) * pow(1 - t, n - k) * controlPoints[k].y;
    }
    return glm::vec2(x, y);
}

void BezierCurveComponent::addControlPoint(float x, float y)
{
	for (int i = 0; i < controlPoints.size(); i++)
    {
		if (controlPoints[i].x == x && controlPoints[i].y == y)
			return;
    }

	controlPoints.push_back(glm::vec2(x, y));

	if (controlPoints.size() > 1)
		updateCurve();
}

int BezierCurveComponent::getSelectedPoint(float mouseX, float mouseY)
{
	int selectedPointIdx = -1;

	for (int i = 0; i < controlPoints.size(); i++)
    {
		// Formule de distance : https://en.wikipedia.org/wiki/Euclidean_distance#Two_dimensions
		float clickDistance = sqrt((controlPoints[i].x - mouseX) * (controlPoints[i].x - mouseX) + (controlPoints[i].y - mouseY) * (controlPoints[i].y - mouseY));
		
		if (clickDistance <= CONTROL_POINT_RADIUS)
		{
			selectedPointIdx = i;
			break;
		}
    }

	return selectedPointIdx;
}

void BezierCurveComponent::draw()
{
	if (vertices.size() > 1)
	{
		for (int i = 0; i < vertices.size() - 1; i++)
		{
			ofFill();
			ofSetColor(outlineColor);
			ofPushStyle();

			// CREDITS: https://forum.openframeworks.cc/t/ofsetlinewidth-maximum-value/3751/4
			glm::highp_vec3 startPos = glm::highp_vec3(vertices[i].x, vertices[i].y, 0);
			glm::highp_vec3 endPos = glm::highp_vec3(vertices[i + 1].x, vertices[i + 1].y, 0);

			lineDrawer.drawLine(startPos, endPos, lineWidth);

			ofPopStyle();
		}
	}

	for (int i = 0; i < controlPoints.size(); i++)
    {
        ofPushStyle();
		ofFill();
        ofSetColor(outlineColor);
        ofDrawCircle(controlPoints[i].x, controlPoints[i].y, CONTROL_POINT_RADIUS);

        ofPopStyle();
    }
}
