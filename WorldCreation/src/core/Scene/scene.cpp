#include "scene.h"

Scene::Scene()
{
}

void Scene::addEntity(Entity entity)
{
	sceneEntities.addEntity(entity);
}


// if parent == nullptr, creates entity at root
// if parent exists, creates entity if finds parent
// if parent doesn't exist does nothing
Entity* Scene::addEntity(string name, Entity* parent)
{
	if (parent == nullptr)
	{
		Entity* entity = new Entity(name, parent, &sceneEntities);
		sceneEntities.addEntity(*entity);
		return entity;
	}
	else if(sceneEntities.entityExist(parent))
	{
		Entity* entity = new Entity(name, parent, &sceneEntities);
		sceneEntities.addEntityAt(*entity, parent);
		return entity;
	}
	
	return nullptr;
}

bool Scene::removeEntity(Entity* entity)
{
	return sceneEntities.removeEntity(entity);
}

bool Scene::moveEntityAt(Entity* movedEntity, Entity* parent)
{
	return sceneEntities.moveEntityAt(movedEntity, parent);
}

std::vector<Entity*> Scene::getLightEntities()
{
	std::vector<Entity*> lightEntities;

	for (Entity* entity : sceneEntities.entities)
	{
		if (entity->getDrawableComponent() != nullptr)
		{
			if (LightComponent* lightComponent = dynamic_cast<LightComponent*>(entity->getDrawableComponent()))
			{
				lightEntities.push_back(entity);
			}
		}
	}

	return lightEntities;
}