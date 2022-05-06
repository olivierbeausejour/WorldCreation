#include "hierarchyPanel.h"
#include "interface/interface.h"

Entity* Interface::selectedEntity;

void HierarchyPanel::setCurrentScene(Scene* currentScene)
{
	this->currentScene = currentScene;
}

void HierarchyPanel::draw()
{
	EditorUI::Begin("Hierarchy", NULL);
	setDragDropTarget(nullptr, nullptr, EditorUI::GetCurrentWindow());
	AddPopupContext();
	
	if (&currentScene->getEntityTree()->getRootEntities() == nullptr) return; 

	drawRecursiveEntities(currentScene->getEntityTree()->getRootEntities());

	EditorUI::End();
}

void HierarchyPanel::drawRecursiveEntities(std::vector<Entity*> entities)
{
	static ImGuiTreeNodeFlags defaultFlags =
		ImGuiTreeNodeFlags_OpenOnArrow |
		ImGuiTreeNodeFlags_OpenOnDoubleClick |
		ImGuiTreeNodeFlags_SpanAvailWidth |
		ImGuiTreeNodeFlags_SpanFullWidth;

	for (int i = 0; i < entities.size(); i++)
	{
		ImGuiTreeNodeFlags nodeFlags = defaultFlags |
			(entities.at(i)->getChildren().empty() ? ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet : 0) |
			(entities.at(i) == Interface::getSelectedEntity() ? ImGuiTreeNodeFlags_Selected : 0);

		bool open = EditorUI::TreeNodeEx((void*)(intptr_t)i, nodeFlags, entities.at(i)->name.c_str(), i);

		AddPopupContext(entities.at(i));

		if (ImGui::IsItemClicked())
		{
			Interface::setSelectedEntity(entities.at(i));
		}

		setDragDropTarget(&entities, &i);

		if (EditorUI::BeginDragDropSource())
		{
			unsigned int entityIndex = currentScene->getEntityTree()->getEntityIndex(entities.at(i));
			EditorUI::SetDragDropPayload(HIERARCHY_PANEL_DRAG_PAYLOAD, &entityIndex, sizeof(unsigned int));
			EditorUI::Text("Move %s", entities.at(i)->name.c_str());
			EditorUI::EndDragDropSource();
		}

		if (open)
		{
			drawRecursiveEntities(entities.at(i)->getChildren());

			EditorUI::TreePop();
		}
	}
}

void HierarchyPanel::moveSceneEntity(Entity* movedEntity, Entity* targetEntity)
{
	currentScene->moveEntityAt(movedEntity, targetEntity);
}

void HierarchyPanel::addSceneEntity(Entity* parent/*= nullptr*/)
{
	currentScene->addEntity("new entity", parent);
}

void HierarchyPanel::removeSceneEntity(Entity* entity)
{
	currentScene->removeEntity(entity);
}

void HierarchyPanel::setDragDropTargetInternal(vector<Entity*>* entities, const int* i)
{
	if (const ImGuiPayload* payload = EditorUI::AcceptDragDropPayload(HIERARCHY_PANEL_DRAG_PAYLOAD))
	{
		if (Entity* draggedEntity = currentScene->getEntityTree()->getEntityAtIndex(*(const unsigned int*)payload->Data))
		{
			if (entities && i && draggedEntity != entities->at(*i))
			{
				moveSceneEntity(draggedEntity, entities->at(*i));
			}
			else if(draggedEntity->getParent() != nullptr)
			{
				moveSceneEntity(draggedEntity, nullptr);
			}
		}
	}
}

void HierarchyPanel::setDragDropTarget(vector<Entity*>* entities, const int* i, ImGuiWindow* currentWindow)
{
	if (!currentWindow && EditorUI::BeginDragDropTarget())
	{
		setDragDropTargetInternal(entities, i);

		EditorUI::EndDragDropTarget();
	}
	else if(currentWindow != nullptr)
	{
		const ImRect windowBB(currentWindow->DC.CursorPos, currentWindow->DC.CursorPos + currentWindow->Size);

		if(EditorUI::BeginDragDropTargetCustom(windowBB, currentWindow->ID))
		{
			EditorUI::GetCurrentContext()->DragDropActive = true;

			setDragDropTargetInternal(entities, i);

			EditorUI::EndDragDropTarget();
		}
	}
}

void HierarchyPanel::AddPopupContext(Entity* currentEntity)
{
	if (currentEntity == nullptr ? EditorUI::BeginPopupContextWindow() : EditorUI::BeginPopupContextItem())
	{
		// Renaming
		if (currentEntity != nullptr)
		{
			char name[255];
			strcpy(name, currentEntity->name.c_str());
			EditorUI::Text("Edit name:");

			if (EditorUI::InputText("##edit", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				EditorUI::CloseCurrentPopup();
			}
			string newName = name;
			currentEntity->setName(newName);
		}

		// Create entity
		if (EditorUI::Button("Create entity"))
		{
			addSceneEntity(currentEntity);
			EditorUI::CloseCurrentPopup();
		}
		
		// delete entity
		if (currentEntity != nullptr && EditorUI::Button("Delete entity"))
		{
			//TODO changer pour setSelectedANull seulement si currentEntity 
			//est le parent du selectedEntity
			//if (currentEntity ou un de ses enfants == Interface::getSelectedEntity())
			//{
			//	  Interface::setSelectedEntity(nullptr);
			//}
			Interface::setSelectedEntity(nullptr);
			removeSceneEntity(currentEntity);
			Interface::setSelectedEntity(nullptr);
			EditorUI::CloseCurrentPopup();
		}

		EditorUI::EndPopup();
	}
}
