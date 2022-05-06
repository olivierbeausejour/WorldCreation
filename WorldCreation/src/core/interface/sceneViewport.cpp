#include "sceneViewport.h"


void SceneViewport::update()
{
	if (sceneViewportHovered)
	{
		auto mouseCommands = MouseCommands::getComputedCommands(name);
		
		editorCameraDragging = false;
		if (mouseCommands.mouseButton == MouseCommandsButtonsEnum::MIDDLE || 
			mouseCommands.mouseButton == MouseCommandsButtonsEnum::RIGHT)
		{

			editorCameraDragging = true;
			editorCamera.mouseDragged(
				screenMousePosition.x - lastScreenMousePosition.x,
				-(screenMousePosition.y - lastScreenMousePosition.y),
				mouseCommands.mouseButton);
		}

		editorCamera.update();
	}
}

// Also records inputs from ImGui to control the camera because it's the only place we can access imgui functions
void SceneViewport::draw()
{
	EditorUI::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
	if (!EditorUI::Begin(name.data(), !canClose ? NULL : &isOpen, ImGuiWindowFlags_MenuBar))
	{
		EditorUI::PopStyleVar();
		// Window is collapsed
		EditorUI::End();
		return;
	}

	if (EditorUI::BeginMenuBar())
	{
		EditorUI::SetNextItemWidth(100);
		const char* orthographicSelectableTitle = "orthographic";
		EditorUI::PushStyleColor(ImGuiCol_Header, ImVec4(0.223f, 0.458f, 0.694f, 1.0f));
		if (EditorUI::Selectable(orthographicSelectableTitle, &orthographicView, 0, EditorUI::CalcTextSize(orthographicSelectableTitle)))
		{
			orthographicView ? editorCamera.setOrthographic() : editorCamera.setPerspective();
		}
		EditorUI::PopStyleColor();
		EditorUI::EndMenuBar();
	}

	sceneWindowWorkRect = EditorUI::GetCurrentWindow()->WorkRect;
	lastScreenMousePosition = screenMousePosition;
	screenMousePosition = EditorUI::GetMousePos();
	ImVec2 windowMousePosition = screenMousePosition - sceneWindowWorkRect.Min;
	viewportFocused = EditorUI::IsWindowFocused();

	relativeViewportMousePosition.x = std::max(0.0f, std::min(windowMousePosition.x, sceneWindowWorkRect.Max.x - sceneWindowWorkRect.Min.x));
	relativeViewportMousePosition.y = std::max(0.0f, std::min(windowMousePosition.y, sceneWindowWorkRect.Max.y - sceneWindowWorkRect.Min.y));

	sceneViewportHovered = isSceneViewportHovered(windowMousePosition.x, windowMousePosition.y);
	
	ImVec2 newMousePosition = screenMousePosition;
	if (editorCameraDragging)
	{
		CustomCursor::setCursorType(CustomCursorEnum::HAND);
		bool changeMousePosition = false;
		if (relativeViewportMousePosition.x < 5.0f)
		{
			changeMousePosition = true;
			newMousePosition.x = sceneWindowWorkRect.Max.x - SCENE_VIEWPORT_MOUSE_CONTROL_WINDOW_MARGIN;
			lastScreenMousePosition.x = newMousePosition.x - (screenMousePosition.x - lastScreenMousePosition.x);
		}
		else if (relativeViewportMousePosition.x > sceneWindowWorkRect.Max.x - sceneWindowWorkRect.Min.x - SCENE_VIEWPORT_MOUSE_CONTROL_WINDOW_MARGIN)
		{
			changeMousePosition = true;
			newMousePosition.x = sceneWindowWorkRect.Min.x + SCENE_VIEWPORT_MOUSE_CONTROL_WINDOW_MARGIN;
			lastScreenMousePosition.x = newMousePosition.x - (screenMousePosition.x - lastScreenMousePosition.x);
		}
		if (relativeViewportMousePosition.y < 3.0f)
		{
			changeMousePosition = true;
			newMousePosition.y = sceneWindowWorkRect.Max.y - SCENE_VIEWPORT_MOUSE_CONTROL_WINDOW_MARGIN;
			lastScreenMousePosition.y = newMousePosition.y - (screenMousePosition.y - lastScreenMousePosition.y);
		}
		else if (relativeViewportMousePosition.y > sceneWindowWorkRect.Max.y - sceneWindowWorkRect.Min.y - SCENE_VIEWPORT_MOUSE_CONTROL_WINDOW_MARGIN)
		{
			changeMousePosition = true;
			newMousePosition.y = sceneWindowWorkRect.Min.y + SCENE_VIEWPORT_MOUSE_CONTROL_WINDOW_MARGIN;
			lastScreenMousePosition.y = newMousePosition.y - (screenMousePosition.y - lastScreenMousePosition.y);
		}

		if (changeMousePosition)
		{
			SetCursorPos((int)newMousePosition.x, (int)newMousePosition.y);
			screenMousePosition = newMousePosition;
		}
	}
	
	auto texture = fbo->getTexture();
	auto sceneViewSize = EditorUI::GetContentRegionAvail();
	
	viewWidth = sceneViewSize.x;
	viewHeight = sceneViewSize.y;
	EditorUI::Image((ImTextureID)(uintptr_t)texture.getTextureData().textureID, sceneViewSize);

	MouseCommands::computeCommands(name);

	if (sceneViewportHovered)
	{
		editorCamera.mouseScrolled(-EditorUI::GetIO().MouseWheel);
	}
	EditorUI::PopStyleVar();
	EditorUI::End();
}

bool SceneViewport::isSceneViewportHovered(float x, float y)
{
	if (x < 0.0f)
	{
		return false;
	}
	else if (x > sceneWindowWorkRect.Max.x - sceneWindowWorkRect.Min.x)
	{
		return false;
	}
	if (y < 0.0f)
	{
		return false;
	}
	else if (y > sceneWindowWorkRect.Max.y - sceneWindowWorkRect.Min.y)
	{
		return false;
	}
	return true;
}
