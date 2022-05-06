#pragma once

#include "wcpch.h"

#include "interface/hierarchyPanel.h"
#include "interface/propertyPanels/entityPropertiesPanel.h"
#include "interface/utilityPanels/canvasUtilityPanel.h"
#include "interface/utilityPanels/sceneUtilityPanel.h"
#include "interface/EnvironnementPanel/environnementPanel.h"
#include "interface/sceneViewport.h"
#include "interface/canvasViewport.h"
#include "interface/customCursor.h"
#include "interface/EditorCamera/editorCamera.h"

#include "commands/commandFactory.h"

#include "inputs/mouseCommands.h"
#include "inputs/inputs.h"

class Interface
{

public:

	Interface();
	~Interface();

	void setup(ofAppGLFWWindow* ofWindow, Scene& scene, std::shared_ptr<CanvasRenderer> canvasRenderer);
	void preDraw();
	void postDraw();
	void drawSceneViewport(ofFbo* sceneFramebuffer, SceneViewport* sceneViewport);
	void drawCanvasViewport(ofFbo* canvasFramebuffer);

	void update();

	static bool executeCommand(const Action& action, void* object);

	SceneViewport* getMainSceneViewport() { return mainSceneViewport; }
	CanvasViewport* getCanvasViewport() { return canvasViewport; }

	std::unique_ptr<CanvasUtilityPanel>& getCanvasUtilityPanel() { return canvasUtilityPanel; }
	std::unique_ptr<SceneUtilityPanel>& getSceneUtilityPanel() { return sceneUtilityPanel; }

	static Entity* getSelectedEntity();
	static void setSelectedEntity(Entity* entity);

	static std::string getErrorMessage(const Action& action);

	void addSceneViewport(SceneViewport* viewport);
	void removeSceneViewport(SceneViewport* viewport);

	vector<SceneViewport*>& getSceneViewports() { return sceneViewports; }

private:

	void setColors();
	void showMainMenu();
	void showErrorPopup(const Action & action);
	bool loadImageFromFile(Action& menuAction);

	static Entity* selectedEntity;

	std::shared_ptr<CanvasRenderer> canvasRenderer;

	std::unique_ptr<HierarchyPanel> hierarchyPanel;
	std::unique_ptr<EntityPropertiesPanel> selectedEntityPanel;
	std::unique_ptr<SceneUtilityPanel> sceneUtilityPanel;
	std::unique_ptr<CanvasUtilityPanel> canvasUtilityPanel;
	std::unique_ptr<EnvironnementPanel> environnementPanel;

	SceneViewport* mainSceneViewport;
	CanvasViewport* canvasViewport;

	vector<SceneViewport*> sceneViewports;

	EditorUI::Gui gui;
	static CommandFactory commandFactory;

	ImFont* currentFont;

};
