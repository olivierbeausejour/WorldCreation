#include "transformComponent.h"
#include "entity/entity.h"


TransformComponent::TransformComponent(Entity* entityTransform) : entityTransform(entityTransform)
{
	reset();
}

glm::vec3 TransformComponent::getGlobalPosition()
{
	if (entityTransform != nullptr && entityTransform->getParent() != nullptr)
	{
		return this->localPosition + entityTransform->getParent()->getTransform()->getGlobalPosition();
	}
	else
	{
		return getLocalPosition();
	}
}

void TransformComponent::reset()
{
	localPosition = glm::vec3(0, 0, 0);
	localScale = glm::vec3(1, 1, 1);
	localEulerAngles = glm::vec3(0, 0, 0);
}

glm::vec3 TransformComponent::getGlobalScale()
{
	if (entityTransform != nullptr && entityTransform->getParent() != nullptr)
	{
		return this->localScale * entityTransform->getParent()->getTransform()->getGlobalScale();
	}
	else
	{
		return getLocalScale();
	}
}

glm::vec3 TransformComponent::getGlobalEulerAngles()
{
	if (entityTransform != nullptr && entityTransform->getParent() != nullptr)
	{
		return this->localEulerAngles + entityTransform->getParent()->getTransform()->getGlobalEulerAngles();
	}
	else
	{
		return getLocalEulerAngles();
	}
}

void TransformComponent::setLocalEulerAngles(glm::vec3 eulerAngles)
{
	this->localEulerAngles = eulerAngles;
	this->localRotation = glm::quat(localEulerAngles);
}

glm::quat TransformComponent::getGlobalRotation()
{
	if (entityTransform != nullptr && entityTransform->getParent() != nullptr)
	{
		return this->localRotation * this->entityTransform->getParent()->getTransform()->getGlobalRotation();
	}
	else
	{
		return getLocalRotation();
	}
}

void TransformComponent::setTransformAsChild(Entity* parent)
{
	glm::vec3 parentGlobalPosition = parent->getTransform()->getGlobalPosition();
	localPosition = getGlobalPosition() - parentGlobalPosition;

	setLocalEulerAngles(getGlobalEulerAngles() - parent->getTransform()->getGlobalEulerAngles());
}

void TransformComponent::setTransformAsRoot()
{
	localPosition = getGlobalPosition();
	setLocalEulerAngles(getGlobalEulerAngles());
}