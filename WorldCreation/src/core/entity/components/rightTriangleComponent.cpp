#include "rightTriangleComponent.h"

void RightTriangleComponent::setSize(const glm::vec3 size)
{
	Shape2dComponent::setSize(size);

	vertices.push_back(ofVec2f(transform->getLocalPosition().x - (size.x / 2), (transform->getLocalPosition().y - (size.y / 2))));
	vertices.push_back(ofVec2f(transform->getLocalPosition().x + (size.x / 2), (transform->getLocalPosition().y + (size.y / 2))));
	vertices.push_back(ofVec2f(transform->getLocalPosition().x - (size.x / 2), (transform->getLocalPosition().y + (size.y / 2))));
}
