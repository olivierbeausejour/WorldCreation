#pragma once

#include "wcpch.h"

#include "renderer/abstractRenderer.h"

#include "interface/abstractViewport.h"

class RenderTextureComponent : public TextureComponent
{

public:

    RenderTextureComponent() : TextureComponent(TextureTypesEnum::RENDER), source(nullptr)
    {
        textureToUpdate = true;
    };
    ~RenderTextureComponent() {};

    AbstractViewport* getSource() const { return source; }
    void setSource(AbstractViewport* source)
    {  
        this->source = source; 
    }

    bool update(const bool forceUpdate) override;

protected:

    ofImage image;
    AbstractViewport* source;

};