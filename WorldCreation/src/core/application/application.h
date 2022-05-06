#pragma once

#include "wcpch.h"

#include "interface/interface.h"

#include "renderer/sceneRenderer.h"
#include "renderer/canvasRenderer.h"

#include "inputs/inputs.h"

#include "entity/components/cubeComponent.h"
#include "entity/components/coneComponent.h"
#include "entity/components/planeComponent.h"
#include "entity/components/sphereComponent.h"

#include "Scene/scene.h"

class Application : public ofBaseApp
{

public:

	Application();
	~Application() {};

	void setup();
	void update();
	void draw();

	int getWindowWidth() { return windowWidth; };
	int getWindowHeight() { return windowHeight; };

private:
	ofAppGLFWWindow* ofWindow;

	Interface interface;
	std::shared_ptr<SceneRenderer> sceneRenderer;
	std::shared_ptr<CanvasRenderer> canvasRenderer;
	Inputs inputs;

	Scene scene2d;
	Scene scene3d;

	int windowWidth;
	int windowHeight;

	void updatePixelsOccurrence();
};
