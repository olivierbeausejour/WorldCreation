#pragma once

#include "wcpch.h"

#include "Scene/scene.h"
#include "entity/entity.h"


#define HIERARCHY_PANEL_DRAG_PAYLOAD "HIERARCHY_PANEL_DRAG_PAYLOAD"

class HierarchyPanel
{

public:

	HierarchyPanel() : currentScene(nullptr) {};
	~HierarchyPanel() {};

	void setCurrentScene(Scene* currentScene);

	void draw();
	void drawRecursiveEntities(std::vector<Entity*> entities);

	void moveSceneEntity(Entity* movedEntity, Entity* targetEntity);
	void addSceneEntity(Entity* parent = nullptr);
	void removeSceneEntity(Entity* entity);

private:

	void setDragDropTargetInternal(vector<Entity*>* entities, const int* i);
	void setDragDropTarget(vector<Entity*>* entities, const int* i, ImGuiWindow* currentWindow = nullptr);

	void AddPopupContext(Entity* currentEntity = nullptr);

	Scene* currentScene;

};



