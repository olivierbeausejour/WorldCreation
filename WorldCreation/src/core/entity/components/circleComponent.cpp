#include "circleComponent.h"

void CircleComponent::setSize(const glm::vec3 size)
{
	float smallestSide = MathHelper::closestToZero(size.x, size.y);
	float sideX = MathHelper::getSameSignValue(size.x, smallestSide);
	float sideY = MathHelper::getSameSignValue(size.y, smallestSide);

	Shape2dComponent::setSize(ofVec3f(sideX, sideY));

	float angleIncrement = (360.0f / circleResolution) * M_PI / 180.0f;
	for (size_t k = 0; k < circleResolution; k++)
	{
		float vertexX = transform->getLocalPosition().x + (cos(angleIncrement * static_cast<float>(k)) * (this->size.x / 2));
		float vertexY = transform->getLocalPosition().y + (sin(angleIncrement * static_cast<float>(k)) * (this->size.y / 2));
		vertices.push_back(ofVec2f(vertexX, vertexY));
	}
}
