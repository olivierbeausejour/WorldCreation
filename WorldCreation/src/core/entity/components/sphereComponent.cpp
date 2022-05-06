#include "sphereComponent.h"

SphereComponent::SphereComponent() : Shape3dComponent()
{
	ofSpherePrimitive sphere;
	sphere.setRadius(0.5f);
	this->shape = sphere;
	this->orientedBoundingBox = glm::vec3(0.5f, 0.5f, 0.5f);
}