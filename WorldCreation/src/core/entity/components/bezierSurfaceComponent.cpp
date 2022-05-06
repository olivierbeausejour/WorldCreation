#include "bezierSurfaceComponent.h"

BezierSurfaceComponent::BezierSurfaceComponent() : Shape3dComponent()
{
	// initialiser points
	for (int i = 0; i < 4; i++)
	{
		controlPoints.push_back(vector<glm::vec3>(4));
	}

	controlPoints[0][0] = glm::vec3(0, 2, -1);
	controlPoints[1][0] = glm::vec3(0, 4.5, 1);
	controlPoints[2][0] = glm::vec3(0, 2.5, 3);
	controlPoints[3][0] = glm::vec3(0, 3, 4);

	controlPoints[0][1] = glm::vec3(1, 2, -1);
	controlPoints[1][1] = glm::vec3(1, 1.5, 1);
	controlPoints[2][1] = glm::vec3(1, 2.5, 2);
	controlPoints[3][1] = glm::vec3(1, 1, 3);

	controlPoints[0][2] = glm::vec3(2, 2, -1);
	controlPoints[1][2] = glm::vec3(2, 3.5, 1);
	controlPoints[2][2] = glm::vec3(2, 2.5, 3);
	controlPoints[3][2] = glm::vec3(2, 1.5, 3);

	controlPoints[0][3] = glm::vec3(3, 3.5, 0);
	controlPoints[1][3] = glm::vec3(3, 1.5, 1);
	controlPoints[2][3] = glm::vec3(3, 2, 2);
	controlPoints[3][3] = glm::vec3(3, 4, 4);

	resetSurface();

	this->orientedBoundingBox = glm::vec3(0, 0, 0);
}

// Notes de cours p.224
glm::vec3 BezierSurfaceComponent::interpolateBezier(float u, float v)
{
	int n = controlPoints.size() - 1;
	int m = controlPoints[0].size() - 1;
	float x = 0;
	float y = 0;
	float z = 0;
	for (int i = 0; i <= n; i++)
	{
		for (int j = 0; j <= m; j++)
		{
			x += MathHelper::bernstein(i, n, u) * MathHelper::bernstein(j, m, v) * controlPoints[i][j].x;
			y += MathHelper::bernstein(i, n, u) * MathHelper::bernstein(j, m, v) * controlPoints[i][j].y;
			z += -MathHelper::bernstein(i, n, u) * MathHelper::bernstein(j, m, v) * controlPoints[i][j].z;
		}
	}
	return glm::vec3(x, y, z);
}

void BezierSurfaceComponent::resetSurface()
{
	shape = of3dPrimitive();
	ofMesh *surfaceMesh = shape.getMeshPtr();
	surfaceMesh->setMode(OF_PRIMITIVE_LINES);
	surfaceMesh->enableIndices();

	float delta = 0.03f;
	for (float u = 0; u <= 1; u += delta)
	{
		for (float v = 0; v <= 1; v += delta)
		{
			surfaceMesh->addVertex(interpolateBezier(u, v));
		}
	}
}

void BezierSurfaceComponent::draw()
{
	Shape3dComponent::draw();
	int n = controlPoints.size() - 1;
	int m = controlPoints[0].size() - 1;
	for (int i = 0; i <= n; i++)
	{
		for (int j = 0; j <= m; j++)
		{
			//-y puisque, pour une raison ou une autre, le y est inversé pour les points de ctrl dans la scène
			ofDrawSphere(transform->getGlobalScale().x * (transform->getGlobalPosition().x + controlPoints[i][j].x),
						 transform->getGlobalScale().y * (transform->getGlobalPosition().y + -controlPoints[i][j].y),
						 transform->getGlobalScale().z * (transform->getGlobalPosition().z + controlPoints[i][j].z),
						 0.05f);
		}
	}
}
