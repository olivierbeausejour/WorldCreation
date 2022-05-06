#include "ellipseComponent.h"

void EllipseComponent::setSize(const glm::vec3 size)
{
	Shape2dComponent::setSize(size);

	float angleIncrement = (360.0f / circleResolution) * M_PI / 180.0f;
	for (size_t k = 0; k < circleResolution; k++)
	{
		float vertexX = transform->getLocalPosition().x + (cos(angleIncrement * static_cast<float>(k)) * (size.x / 2));
		float vertexY = transform->getLocalPosition().y + (sin(angleIncrement * static_cast<float>(k)) * (size.y / 2));
		vertices.push_back(ofVec2f(vertexX, vertexY));
	}
}
