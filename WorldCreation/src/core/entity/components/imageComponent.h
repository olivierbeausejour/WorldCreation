#pragma once

#include "wcpch.h"

#include "drawableComponent.h"

class ImageComponent : public DrawableComponent
{

public:

    ImageComponent(ofImage* image) : DrawableComponent(), image(image) {};
    ~ImageComponent() 
    {
        delete image;
    };

    void draw() override 
    { 
        ofPushStyle();
        ofSetColor(255, 255);
        image->draw(transform->getLocalPosition().x, transform->getLocalPosition().y, size.x, size.y); 
        ofPopStyle();
    };

private:

    ofImage* image;
};
