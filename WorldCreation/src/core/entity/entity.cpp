#include "entity.h"
#include "Scene/entityTree.h"

Entity::Entity(string name, /*= ""*/ Entity* parent /*=nullptr*/, EntityTree* entityTree/* = nullptr*/) :
	drawableComponent(nullptr)
{
	this->name = name;
	this->parent = parent;
	this->entityTree = entityTree;
	if (parent != nullptr)
	{
		this->parent->getChildren().push_back(this);
	}

	this->transform = TransformComponent(this);
}

Entity::~Entity()
{
	// TODO: Fix memory leak
	//delete transform;
	delete drawableComponent;
	for (auto entity : children)
	{
		if (entity != nullptr)
		{
			Entity* refPtr = entity;
			delete entity;
			if (entityTree != nullptr) 
			{
				entityTree->entities.at((entityTree->getEntityIndex(refPtr))) = nullptr;
			}
		}
	}
}

void Entity::setDrawableComponent(DrawableComponent* drawableComponent)
{
	if (drawableComponent == nullptr)
	{
		delete(this->drawableComponent);
	}
	else
	{
		if (this->drawableComponent != nullptr)
		{
			drawableComponent->setColor(this->drawableComponent->getColor());
		}
    
		drawableComponent->setTransform(&(this->transform));
		delete(this->drawableComponent);
	}

	this->drawableComponent = drawableComponent;
}

void Entity::drawDrawableComponent()
{
	if (drawableComponent != nullptr)
	{
		this->drawableComponent->draw();
	}	
}
