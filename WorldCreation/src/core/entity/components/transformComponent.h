#pragma once
#include "wcpch.h"

class Entity;

class TransformComponent
{

public:

	TransformComponent() : 
		localPosition(glm::vec3(0.0f)),	
		localScale(glm::vec3(0.0f)), 
		localRotation(glm::vec3(0.0f)), 
		localEulerAngles(glm::vec3(0.0f)), 
		entityTransform(nullptr) {}
	TransformComponent(Entity* entityTransform);
	~TransformComponent() {}

	glm::vec3 getLocalPosition() { return this->localPosition; }
	glm::vec3 getGlobalPosition();
	void setLocalPosition(glm::vec3 position) {	this->localPosition = position;	}
	void setLocalPosition(float x, float y, float z) {	this->localPosition = glm::vec3(x, y, z); }

	glm::vec3 getLocalScale() {	return this->localScale; }
	glm::vec3 getGlobalScale();
	void setLocalScale(glm::vec3 scale)	{ this->localScale = scale;	}
	void setLocalScale(float x, float y, float z) { this->localScale = glm::vec3(x, y, z); }

	glm::vec3 getLocalEulerAngles() { return this->localEulerAngles; }
	glm::vec3 getGlobalEulerAngles();
	void setLocalEulerAngles(glm::vec3 eulerAngles);
	void setLocalEulerAngles(float x, float y, float z) 
	{ 
		setLocalEulerAngles(glm::vec3(x, y, z));
	}

	glm::quat getLocalRotation() { return this->localRotation; }
	glm::quat getGlobalRotation();
	void setLocalRotation(glm::vec3 localEulerAngles) { this->localRotation = glm::quat(localEulerAngles); }
	void setLocalRotation(glm::quat localRotation) { this->localRotation = localRotation; }

	glm::vec3 getRightDirection() { return glm::rotate(localRotation, glm::vec3(1.0f, 0.0f, 0.0f)); }
	glm::vec3 getUpDirection() { return glm::rotate(localRotation, glm::vec3(0.0f, 1.0f, 0.0f)); }
	glm::vec3 getForwardDirection() { return glm::rotate(localRotation, glm::vec3(0.0f, 0.0f, 1.0f)); }

	void setTransformAsChild(Entity* parent);
	void setTransformAsRoot();


	void reset();

private:

		glm::vec3 localPosition;
		glm::vec3 localScale;
		glm::quat localRotation;
		glm::vec3 localEulerAngles;
		Entity* entityTransform;
};

