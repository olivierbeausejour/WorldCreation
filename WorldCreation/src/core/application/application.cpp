#include "application.h"

#define PLAYMODE

Application::Application()
{
	windowWidth = 1920;
	windowHeight = 1080;

	ofGLFWWindowSettings windowSettings;
	windowSettings.resizable = true;
	windowSettings.title = "World Creation";

	windowSettings.setSize(windowWidth, windowHeight);
	windowSettings.setGLVersion(4, 6);
	
	ofCreateWindow(windowSettings);

	ofWindow = (ofAppGLFWWindow*)ofGetWindowPtr();

	canvasRenderer = std::make_shared<CanvasRenderer>();
	sceneRenderer = std::make_shared<SceneRenderer>();
	glfwMaximizeWindow(ofWindow->getGLFWWindow());

	ofDisableArbTex();
}

void Application::setup()
{
	interface.setup(ofWindow, scene3d, canvasRenderer);

	sceneRenderer->setup(&scene3d);
	sceneRenderer->setRendererWidth(interface.getMainSceneViewport()->getViewWidth());
	sceneRenderer->setRendererHeight(interface.getMainSceneViewport()->getViewHeight());

	canvasRenderer->setup(&scene2d);
	canvasRenderer->setRendererWidth(interface.getCanvasViewport()->getViewWidth());
	canvasRenderer->setRendererHeight(interface.getCanvasViewport()->getViewHeight());
}

void Application::update()
{
	interface.update();

	for (SceneViewport* sceneViewport : interface.getSceneViewports())
	{
		sceneRenderer->updatePostProcessingShader(
			interface.getSceneUtilityPanel()->getExposureFactor(),
			interface.getSceneUtilityPanel()->getGammaFactor(),
			interface.getSceneUtilityPanel()->getToneMappingTechnique());

		sceneRenderer->getLightingRenderer().setLightingModel(interface.getSceneUtilityPanel()->getLightingModel());	
	}

	if (interface.getSceneUtilityPanel()->getIsScreenCapturing())
	{
		string time_stamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");
		string file_name = interface.getSceneUtilityPanel()->getOutputPath() + "/capture" + time_stamp + ".jpg";

		ofPixels pixels;
		sceneRenderer->getFramebufferPtr()->getTexture().readToPixels(pixels);

		ofImage image;
		image.setFromPixels(pixels);
		image.save(file_name);

		interface.getSceneUtilityPanel()->postScreenCapture();
	}

	canvasRenderer->setPrimitiveToDraw(interface.getCanvasUtilityPanel()->getSelectedTool());
	canvasRenderer->setFillColor(interface.getCanvasUtilityPanel()->getFillColor());
	canvasRenderer->setBackgroundColor(interface.getCanvasUtilityPanel()->getBackgroundColor());
	canvasRenderer->setOutlineColor(interface.getCanvasUtilityPanel()->getOutlineColor());
	canvasRenderer->setLineWidth(interface.getCanvasUtilityPanel()->getLineWidth());
	
	EntityTree * entityTree3D = scene3d.getEntityTree();
	for (auto entityItr = entityTree3D->begin(); entityItr != entityTree3D->end(); ++entityItr)
	{
		if (ModelComponent* modelComponent = dynamic_cast<ModelComponent*>(entityItr->getDrawableComponent()))
		{
			modelComponent->update();
		}
	}
}

void Application::draw()
{
	interface.preDraw();

	for (SceneViewport* sceneViewport : interface.getSceneViewports())
	{
		sceneRenderer->setRendererWidth(sceneViewport->getViewWidth());
		sceneRenderer->setRendererHeight(sceneViewport->getViewHeight());
		sceneRenderer->draw(sceneViewport->getSceneViewportEditorCamera());
		interface.drawSceneViewport(sceneRenderer->getFramebufferPtr(), sceneViewport);
	}

	canvasRenderer->setRendererWidth(interface.getCanvasViewport()->getViewWidth());
	canvasRenderer->setRendererHeight(interface.getCanvasViewport()->getViewHeight());
	canvasRenderer->draw();
	interface.drawCanvasViewport(canvasRenderer->getFramebufferPtr());

	if (interface.getCanvasUtilityPanel()->getUpdateHistogram())
	{
		updatePixelsOccurrence();
	}

	interface.postDraw();
}

void Application::updatePixelsOccurrence()
{
	ofPixels canvasPixels;
	ofFbo* canvasFbo = canvasRenderer->getFramebufferPtr();

	canvasFbo->readToPixels(canvasPixels);

	for (size_t i = 0; i < 256; i++)
	{
		interface.getCanvasUtilityPanel()->setRedPixelOccurrence(i, 0.0f);
		interface.getCanvasUtilityPanel()->setGreenPixelOccurrence(i, 0.0f);
		interface.getCanvasUtilityPanel()->setBluePixelOccurrence(i, 0.0f);
	}

	for (size_t i = 0; i < canvasPixels.size(); i += 4)
	{
		ofColor color = canvasPixels.getColor(i);
		float redValue = color.r;
		float greenValue = color.g;
		float blueValue = color.b;

		float currentRedOccurrence = interface.getCanvasUtilityPanel()->getRedPixelOccurrence(redValue);
		interface.getCanvasUtilityPanel()->setRedPixelOccurrence(redValue, currentRedOccurrence + 1);

		float currentGreenOccurrence = interface.getCanvasUtilityPanel()->getGreenPixelOccurrence(greenValue);
		interface.getCanvasUtilityPanel()->setGreenPixelOccurrence(greenValue, currentGreenOccurrence + 1);

		float currentBlueOccurrence = interface.getCanvasUtilityPanel()->getBluePixelOccurrence(blueValue);
		interface.getCanvasUtilityPanel()->setBluePixelOccurrence(blueValue, currentBlueOccurrence + 1);
	}
}