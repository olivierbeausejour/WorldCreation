#pragma once

#include "wcpch.h"

#include "gradientTypesEnum.h"
#include "textureComponent.h"

class GradientTextureComponent : public TextureComponent
{

public:

    GradientTextureComponent() : TextureComponent(TextureTypesEnum::GRADIENT), beginColor(ofFloatColor::red), endColor(ofFloatColor::black), type(GradientTypesEnum::HORIZONTAL) 
    {
        image.allocate(GRADIENT_SIZE, GRADIENT_SIZE, ofImageType::OF_IMAGE_COLOR_ALPHA);
    };

    ~GradientTextureComponent() 
    {
    };

    GradientTypesEnum getType() const { return type; }
    void setType(const GradientTypesEnum type) 
    { 
        if (this->type != type)
        {
            this->type = type;
            textureToUpdate = true;
        }
    }

    ofFloatColor getBeginColor() const { return beginColor; }
    void setBeginColor(const ofFloatColor color) 
    { 
        if (this->beginColor != color)
        {
            beginColor = color;
            textureToUpdate = true;
        }
    }
    
    ofFloatColor getEndColor() const { return endColor; }
    void setEndColor(const ofFloatColor color) 
    { 
        if (this->endColor != color)
        {
            endColor = color;
            textureToUpdate = true;
        }
    }

    bool update(const bool forceUpdate) override;

protected:

    ofImage image;
    static const int GRADIENT_SIZE = 128;

    GradientTypesEnum type;
    ofFloatColor beginColor;
	ofFloatColor endColor;

};