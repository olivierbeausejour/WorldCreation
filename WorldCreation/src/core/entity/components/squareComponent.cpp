#include "squareComponent.h"

void SquareComponent::setSize(const glm::vec3 size)
{
	float smallestSide = MathHelper::closestToZero(size.x, size.y);
	float sideX = MathHelper::getSameSignValue(size.x, smallestSide);
	float sideY = MathHelper::getSameSignValue(size.y, smallestSide);

	Shape2dComponent::setSize(ofVec3f(sideX, sideY));

	vertices.push_back(ofVec2f(transform->getLocalPosition().x - (this->size.x / 2), (transform->getLocalPosition().y - (this->size.y / 2))));
	vertices.push_back(ofVec2f(transform->getLocalPosition().x + (this->size.x / 2), (transform->getLocalPosition().y - (this->size.y / 2))));
	vertices.push_back(ofVec2f(transform->getLocalPosition().x + (this->size.x / 2), (transform->getLocalPosition().y + (this->size.y / 2))));
	vertices.push_back(ofVec2f(transform->getLocalPosition().x - (this->size.x / 2), (transform->getLocalPosition().y + (this->size.y / 2))));
}
