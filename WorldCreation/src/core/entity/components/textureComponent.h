#pragma once

#include "wcpch.h"

#include "textureTypesEnum.h"

class TextureComponent
{

public:

    TextureComponent(TextureTypesEnum type) : type(type), texture(nullptr), textureToUpdate(true) {};
    ~TextureComponent() 
    {
        delete texture;
    };

    TextureTypesEnum getType() const { return type; }

    ofTexture* getTexture() const { return texture; }

    virtual bool update(const bool forceUpdate) = 0;

protected:

    void setTexture(ofTexture* texture)
	{
        textureToUpdate = this->texture != texture;
        if (this->texture != nullptr)
			delete this->texture;
		this->texture = texture;
	}

    TextureTypesEnum type;
    ofTexture* texture;
    bool textureToUpdate;

};