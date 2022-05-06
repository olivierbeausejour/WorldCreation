#pragma once

#include "wcpch.h"

#include "interface/abstractViewport.h"
#include "interface/EditorCamera/editorCamera.h"
#include "interface/customCursor.h"
#include "inputs/mouseCommands.h"

#define SCENE_VIEWPORT_MOUSE_CONTROL_WINDOW_MARGIN 5.0f


class SceneViewport : public AbstractViewport
{

public:

	SceneViewport() :
		AbstractViewport(1000, 1080, "Scene"),
		orthographicView(false),
		editorCameraDragging(false),
		sceneViewportHovered(false),
		viewportFocused(false) { };
	~SceneViewport() { };

	void update();
	void draw() override;
	EditorCamera* getSceneViewportEditorCamera() { return &this->editorCamera; };

private:

	bool isSceneViewportHovered(float x, float y);

	EditorCamera editorCamera;
	bool orthographicView;
	bool editorCameraDragging;

	ImRect sceneWindowWorkRect;
	ImVec2 relativeViewportMousePosition;
	ImVec2 screenMousePosition;
	ImVec2 lastScreenMousePosition;
	bool sceneViewportHovered;
	bool viewportFocused;

};
