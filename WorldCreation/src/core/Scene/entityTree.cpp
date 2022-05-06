#include "entityTree.h"
#include "entity/entity.h"


EntityTree::~EntityTree()
{
	// TODO:: MAKE SURE EVERYTHING DELETES
	for (auto entity : entities)
	{
		delete entity;
	}
}

void EntityTree::addEntity(Entity& entity)
{
	rootEntities.push_back(&entity);
	entities.push_back(&entity);
}

void EntityTree::addEntityAt(Entity& childEntity, Entity* parent)
{
	if (entityExist(parent))
	{
		childEntity.setParent(parent);
	}

	if (parent == nullptr) rootEntities.push_back(&childEntity);
	entities.push_back(&childEntity);
}

bool EntityTree::removeEntity(Entity* entity)
{
	return deleteEntity(entity);
}

bool EntityTree::moveEntityAt(Entity* movedEntity, Entity* parent)
{
	if (isEntityChild(movedEntity, parent)) return false;

	for (auto entityItr = begin(); entityItr != end(); ++entityItr)
	{
		Entity* entityPtr = &(*entityItr);
		if (entityPtr == movedEntity)
		{
			if (movedEntity->getParent() == nullptr)
			{
				for (Entity* childEntityPtr : rootEntities)
				{
					if (childEntityPtr == entityPtr)
					{
						rootEntities.erase(
							std::remove(
								rootEntities.begin(),
								rootEntities.end(),
								childEntityPtr),
							rootEntities.end());
						break;
					}
				}
			}
			else
			{
				for (Entity* childEntityPtr : movedEntity->getParent()->getChildren())
				{
					if (childEntityPtr == entityPtr)
					{
						auto& parentEntityChildren = movedEntity->getParent()->getChildren();
						parentEntityChildren.erase(
							std::remove(
								parentEntityChildren.begin(),
								parentEntityChildren.end(),
								childEntityPtr),
							parentEntityChildren.end());
						break;
					}
				}
			}
			
			if(parent == nullptr)
			{
				rootEntities.push_back(movedEntity);
				movedEntity->getTransform()->setTransformAsRoot();
			}
			else
			{
				parent->getChildren().push_back(movedEntity);
				movedEntity->getTransform()->setTransformAsChild(parent);
			}

			movedEntity->setParent(parent);

			return true;
		}
	}

	return false;
}

bool EntityTree::entityExist(Entity* entity)
{
	for (auto entityItr = begin(); entityItr != end(); ++entityItr)
	{
		if (&(*entityItr) == entity)
		{
			return true;
		}
	}

	return false;
}

unsigned int EntityTree::getEntityIndex(Entity* entity)
{
	unsigned int index = 0;
	for (Entity* ent : entities)
	{
		if (ent == entity)
		{
			return index;
		}
		index++;
	}
	return -1;
}

Entity* EntityTree::getEntityAtIndex(const unsigned int index)
{
	if (entities.size() > index) return entities.at(index);
	return nullptr;
}

bool EntityTree::isEntityChild(Entity* entity, Entity* child)
{
	if (!child) return false;

	for (auto childEntity : entity->getChildren())
	{
		if (childEntity == child) return true;
		if (!childEntity->getChildren().empty())
		{
			if (isEntityChild(childEntity, child)) return true;
		}
	}

	return false;
}

// Calls the destructor of the entity and all the children that it can contains and removes them from rootEntities and entities
bool EntityTree::deleteEntity(Entity* entity)
{
	for (auto entityItr = begin(); entityItr != end(); ++entityItr)
	{
		Entity* entityPtr = &(*entityItr);
		if (entityPtr == entity)
		{
			vector<Entity*>* deleteFrom;
			if (entityPtr->getParent() != nullptr)
			{
				deleteFrom = &entityPtr->getParent()->getChildren();
			}
			else
			{
				deleteFrom = &rootEntities;
			}
			deleteFrom->erase(std::remove(deleteFrom->begin(), deleteFrom->end(), entityPtr), deleteFrom->end());

			entities.at((getEntityIndex(entityPtr))) = nullptr;
			delete entityPtr;

			entities.erase(std::remove(entities.begin(), entities.end(), nullptr), entities.end());
			return true;
		}
	}
	return false;
}
