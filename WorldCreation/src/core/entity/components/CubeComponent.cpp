#include "cubeComponent.h"

CubeComponent::CubeComponent() : Shape3dComponent()
{
	ofBoxPrimitive box;
	box.setWidth(1);
	box.setHeight(1);
	box.setDepth(1);
	this->shape = box;
	this->orientedBoundingBox = glm::vec3(0.5f, 0.5f, 0.5f);
}