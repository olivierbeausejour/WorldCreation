#pragma once

#include "wcpch.h"

#include "Scene/scene.h"
#include "inputs/actions.h"

#include "interface/propertyPanels/componentPropertiesPanel.h"


class EnvironnementPanel : public ComponentPropertiesPanel
{

public:

    EnvironnementPanel();
    ~EnvironnementPanel() = default;

    void setCurrentScene(Scene* currentScene) { this->currentScene = currentScene; }

    void draw();

private:

    bool addCubemapTextureMenu(int textureIndex, string buttonText);

    void setCubemap();
    bool isValidFile(std::string& fileExtension);

    std::vector<std::string> cubemapImagePaths;

    Scene* currentScene;

};