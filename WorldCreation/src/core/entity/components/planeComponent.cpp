#include "planeComponent.h"

PlaneComponent::PlaneComponent() : Shape3dComponent()
{
	ofPlanePrimitive plane;
	plane.set(1, 1);
	this->shape = plane;
	this->orientedBoundingBox = glm::vec3(0.5f, 0.5f, 0);
}