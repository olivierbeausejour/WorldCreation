#pragma once

#include "wcpch.h"

#include "textureComponent.h"

class ImageTextureComponent : public TextureComponent
{

public:

    ImageTextureComponent() : TextureComponent(TextureTypesEnum::IMAGE), imagePath(""), image(nullptr) {};
    ~ImageTextureComponent() 
    { 
        delete image;
    };

    string getImagePath() const { return imagePath; }
    void setImagePath(const string imagePath)
    {
        if (this->imagePath != imagePath)
        {
            this->imagePath = imagePath;
            textureToUpdate = true;
        }
    }

    ofImage* getImage() const { return image; }
    void setImage(ofImage *image) 
    {
        if (this->image != image)
        {
            if (this->image != nullptr)
            {
                delete this->image;
            }
            this->image = image;
            textureToUpdate = true;
        }
    }

    bool update(const bool forceUpdate) override;

protected:

    string imagePath = "";
    ofImage *image;

};