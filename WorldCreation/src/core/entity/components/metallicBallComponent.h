#pragma once

#include "wcpch.h"

#include "sphereComponent.h"

class MetallicBallComponent : public SphereComponent
{

public:

    MetallicBallComponent() : SphereComponent(), roughness(0.0f), normalValue(1.0f)
    {
        iblShader.load("resources/shader/ibl/ibl.vert", "resources/shader/ibl/ibl.frag");
        normalMap.load("resources/normal/metal.jpg");
    }
    ~MetallicBallComponent() {}

    float getRoughness() const { return roughness; }
    void setRoughness(const float roughness) 
    { 
        this->roughness = roughness; 
    }

    float getNormalValue() const { return normalValue; }
    void setNormalValue(const float normalValue) 
    { 
        this->normalValue = normalValue; 
    }

    int getMipLevelFromRoughness(const int nbMipLevels);

    void draw(const ofCamera * camera, const unsigned int cubemapTextureId, const int nbMipLevels, const bool isCubemapLoaded);

protected:

    ofShader iblShader;
    ofImage normalMap;

    float roughness;
    float normalValue;

    std::map<unsigned int, std::vector<ofTexture>> cubemapTextures;

};