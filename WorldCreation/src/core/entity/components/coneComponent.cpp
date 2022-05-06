#include "coneComponent.h"

ConeComponent::ConeComponent() : Shape3dComponent()
{
	ofConePrimitive cone;

	cone.set(0.5f, 1, 24, 3);

	this->shape = cone;
	this->orientedBoundingBox = glm::vec3(0.5f, 0.5f, 0.5f);
}