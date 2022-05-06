#pragma once

#include "wcpch.h"

#include "components/transformComponent.h"
#include "components/drawableComponent.h"


class EntityTree;

class Entity
{

public:

	Entity(string name = "", Entity* parent = nullptr, EntityTree* entityTree = nullptr);
	~Entity();

		  vector<Entity*>& getChildren()	   { return children; }
	const vector<Entity*>& getChildren() const { return children; }
		  Entity* getParent()		{ return parent; }
	const Entity* getParent() const { return parent; }
	void setParent(Entity* parentEntity) { parent = parentEntity; }
	TransformComponent* getTransform() { return &(this->transform); }
	DrawableComponent* getDrawableComponent() { return this->drawableComponent; }
	
	void setName(const string name) { this->name = name; }
	bool operator==(const Entity& lhs) { return &lhs == this; }
	bool operator!=(const Entity& lhs) { return &lhs != this; }
	void setDrawableComponent(DrawableComponent* drawableComponent);
	void drawDrawableComponent();

	string name;

private:

	EntityTree* entityTree;
	Entity* parent;
	std::vector<Entity*> children;

	TransformComponent transform;
	DrawableComponent* drawableComponent;
};
