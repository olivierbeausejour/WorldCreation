#pragma once

#include "wcpch.h"

#include "entity/entity.h"


class EntityTree {

public:

	struct Iterator
	{
		using iterator_category = std::forward_iterator_tag;
		using difference_type	= std::ptrdiff_t;
		using value_type		= Entity;
		using pointer			= Entity*;
		using reference			= Entity&;

		Iterator(pointer ptr, vector<Entity*>* rootEntities) :
			ptr(ptr),
			lastLayersPtr(),
			layerCount(0),
			rootEntities(rootEntities),
			currentEntities(rootEntities)
		{
			layerIndexes.push(1);
		};

		reference operator*() const { return *ptr; }
		pointer operator->() { return ptr; }

		Iterator& operator++()
		{
			// entity will always have been returned before we get here, so check if they have children once they're shown
			if (!ptr->getChildren().empty())
			{
				layerCount++;

				// add last layer data
				if (layerCount <= layerIndexes.size())
				{
					layerIndexes.push(0);
					lastLayersPtr.push(currentEntities);
				}

				currentEntities = &ptr->getChildren();

				ptr = ptr->getChildren().at(layerIndexes.top());
				layerIndexes.top()++;
			}
			// no children, move in current layer
			else
			{
				// check if at the end of layer
				if (layerIndexes.top() >= currentEntities->size())
				{
					// finished the layer so move down a layer and loop until we find a ptr or nullptr
					bool foundPtr = false;
					while (!foundPtr)
					{
						// if on first layer, we are 100% sure done because the 
						// layer has ended and we are on the first so no more entities
						if (layerCount == 0)
						{
							ptr = nullptr;

							foundPtr = true;
						}
						// else go down a layer
						else
						{
							layerCount--;
							currentEntities = lastLayersPtr.top();
							lastLayersPtr.pop();
							layerIndexes.pop();
							// check if valid ptr on this layer and use it otherwise loop
							if (layerIndexes.top() < currentEntities->size())
							{
								foundPtr = true;
								ptr = currentEntities->at(layerIndexes.top());
								layerIndexes.top()++;
							}
						}
					}
				}
				// otherwise go to next current layer ptr
				else
				{
					ptr = currentEntities->at(layerIndexes.top());
					layerIndexes.top()++;
				}
			}

			return *this;
		}

		friend bool operator==(const Iterator& a, const Iterator& b) { return a.ptr == b.ptr; };
		friend bool operator!=(const Iterator& a, const Iterator& b) { return a.ptr != b.ptr; };

	private:

		pointer ptr;
		stack<vector<Entity*>*> lastLayersPtr;
		stack<unsigned int> layerIndexes;
		size_t layerCount;
		vector<Entity*>* rootEntities;
		vector<Entity*>* currentEntities;
	};

	EntityTree(){};
	~EntityTree();

	Iterator begin() { return rootEntities.size() > 0 ? Iterator(rootEntities.front(), &rootEntities) : Iterator(nullptr, nullptr); }
	Iterator end()   { return Iterator(nullptr, nullptr); }

	std::vector<Entity*> getRootEntities() { return rootEntities; }
	
	void addEntity(Entity& entity);
	void addEntityAt(Entity& entity, Entity* parent);
	bool removeEntity(Entity* entity);
	bool moveEntityAt(Entity* movedEntity, Entity* parent);
	bool entityExist(Entity* entity);
	unsigned int getEntityIndex(Entity* entity);
	Entity* getEntityAtIndex(const unsigned int index);

	std::vector<Entity*> rootEntities;
	std::vector<Entity*> entities;
	
private: 

	bool isEntityChild(Entity* entity, Entity* child);
	bool deleteEntity(Entity* entity);

};