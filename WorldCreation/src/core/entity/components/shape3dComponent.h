#pragma once

#include "wcpch.h"

#include "drawableComponent.h"
#include "textureComponent.h"
#include "filterComponent.h"

class Shape3dComponent : public DrawableComponent
{

public:

	Shape3dComponent() : textureComponent(nullptr), filterComponent(nullptr), textureComponentUpdated(true), filterComponentUpdated(true) {};
	~Shape3dComponent() 
	{
		delete textureComponent;
		delete filterComponent;
	};

	void draw() override;

	TextureComponent* getTextureComponent() const
	{
		return this->textureComponent;
	}

	void setTextureComponent(TextureComponent* textureComponent)
	{
		if (this->textureComponent != textureComponent)
		{
			if (this->textureComponent != nullptr)
				delete this->textureComponent;
			this->textureComponent = textureComponent;
			textureComponentUpdated = true;
		}
	}

	void mapTexCoordsFromTexture()
	{
		if (textureComponent != nullptr && textureComponent->getTexture() != nullptr)
			shape.mapTexCoordsFromTexture(*textureComponent->getTexture()); 
	}

	FilterComponent* getFilterComponent() const
	{
		return this->filterComponent;
	}

	void setFilterComponent(FilterComponent* filterComponent)
	{
		if (this->filterComponent != filterComponent)
		{
			if (this->filterComponent != nullptr)
				delete this->filterComponent;
			this->filterComponent = filterComponent;
			filterComponentUpdated = true;
		}
	}

protected:

	of3dPrimitive shape;
	TextureComponent* textureComponent;
	FilterComponent* filterComponent;

	bool textureComponentUpdated;
	bool filterComponentUpdated;

	void preDraw() override;

};