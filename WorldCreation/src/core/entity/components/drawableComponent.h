#pragma once
#include "wcpch.h"

#include "transformComponent.h"
#include "textureComponent.h"

class DrawableComponent
{

public:
	DrawableComponent() : transform(nullptr), color(ofColor(50, 200, 50)), renderMode(ofPolyRenderMode::OF_MESH_FILL) {}
	~DrawableComponent() {}

	ofFloatColor getColor() const
	{
		return this->color;
	}

	void setColor(const ofFloatColor color)
	{
		this->color = color;
	}

	glm::vec3 getSize() const
	{
		return this->size;
	}

	virtual void setSize(const glm::vec3 size)
	{
		this->size = size;
	}

	TransformComponent *getTransform() const
	{
		return transform;
	}

	void setTransform(TransformComponent *transform)
	{
		this->transform = transform;
	}

	ofPolyRenderMode getRenderMode() const
	{
		return renderMode;
	}

	void setRenderMode(const ofPolyRenderMode renderMode)
	{
		this->renderMode = renderMode;
	}

	bool getShowDelimitation()
	{
		return this->showDelimitation;
	}

	void setShowDelimitation(bool showDelimitation)
	{
		this->showDelimitation = showDelimitation;
	}

	glm::vec3 getOrientedBoundingBox()
	{
		return this->orientedBoundingBox;
	}

	ofMaterial &getMaterial() { return this->materialComponent; }

	virtual void draw() = 0;

protected:
	ofFloatColor color;
	glm::vec3 size;
	TransformComponent *transform;
	ofPolyRenderMode renderMode;
	bool showDelimitation;
	glm::vec3 orientedBoundingBox;
	ofMaterial materialComponent;

	virtual void preDraw();
	virtual void postDraw();
};
