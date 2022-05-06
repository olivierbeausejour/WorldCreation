#pragma once

#include "wcpch.h"

#include "entity/entity.h"
#include "scene/entityTree.h"
#include "scene/sceneEnvironnement.h"
#include "entity/components/lightComponent.h"

class Scene
{

public:

	Scene();
	~Scene() {};

	EntityTree* getEntityTree() { return &sceneEntities; }

	void addEntity(Entity entity);
	Entity* addEntity(string name ="", Entity* parent = nullptr);
	bool removeEntity(Entity* entity);
	bool moveEntityAt(Entity* movedEntity, Entity* parent);

	std::vector<Entity*> getLightEntities();

	SceneEnvironnement* getSceneEnvironnement() { return &sceneEnvironnement; }

private:

	EntityTree sceneEntities;
	
	SceneEnvironnement sceneEnvironnement;

};