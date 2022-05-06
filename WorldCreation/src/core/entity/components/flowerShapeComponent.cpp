#include "flowerShapeComponent.h"

void FlowerShapeComponent::setSize(const glm::vec3 size)
{
	Shape2dComponent::setSize(size);

	for (size_t k = 0; k < NB_POINTS; k++)
	{
		float vertexAngle = (90.0f + (k * ANGLE_BETWEEN_POINTS)) * M_PI / 180.0f;
		vertices.push_back(ofVec2f(transform->getLocalPosition().x + (cos(vertexAngle) * (size.x / 2)), transform->getLocalPosition().y + (sin(vertexAngle) * (size.y / 2))));
	}
}
