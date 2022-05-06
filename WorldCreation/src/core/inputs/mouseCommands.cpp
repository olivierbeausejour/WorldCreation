#include "mouseCommands.h"

static constexpr float DRAG_DISTANCE_THRESHOLD = 5.0f;

bool MouseCommands::thresholdExceeded = false;
MouseCommandsButtonsEnum MouseCommands::workingMouseButton;
ImVec2 MouseCommands::beginPosAbsolute;
ImVec2 MouseCommands::actualPosAbsolute;
ImVec2 MouseCommands::beginPosRelative;
ImVec2 MouseCommands::actualPosRelative;

map<string, MouseCommands::MouseCommandsDto> MouseCommands::computedCommands;
map<string, bool> MouseCommands::windowsProgress;

//IMPORTANT TO KNOW : As of know, only DOCKED windows work with this function
void MouseCommands::computeCommands(const string windowName)
{
	actualPosAbsolute = ImVec2(ofGetMouseX(), ofGetMouseY());
	actualPosRelative = EditorUI::GetMousePos() - EditorUI::GetWindowPos() - EditorUI::GetCursorStartPos();

	//Manage mouse up
	if (EditorUI::IsMouseReleased(mapToImGuiMouseButtons(workingMouseButton)) && isWindowInProgress(windowName))
	{
		windowsProgress.insert_or_assign(windowName, false);

		MouseCommands::MouseCommandsDto inboundCommand = computeWorkingCommands(windowName);

		thresholdExceeded = false;
		beginPosAbsolute = ImVec2();
		actualPosAbsolute = ImVec2();

		computedCommands.insert_or_assign(windowName, inboundCommand);
		return;
	}

	ImGuiContext* currentContext = EditorUI::GetCurrentContext();

	//If no window is hovered, and mouse is not doing any actions, then no commands are being made
	if (!isWindowInProgress(windowName) && !EditorUI::IsAnyWindowHovered())
		return;

	//If the current context does not target the same window being hovered by the cursor, then context is not pointing to the
	//right window yet (MouseCommandsEnum::None).
	if (!isWindowInProgress(windowName) && string(currentContext->HoveredWindow->Name).compare(windowName) != 0)
		return;
	
	//If the function manages to come to this point, a MouseCommands::computeCommands() was made in the Draw() function of a certain
	//window, and it is hovered right now. Because of this, mouse commands are able to be made.

	//Manage mouse down
	//TODO: MAKE THIS CLEANER IN THE FUTURE
	bool windowsInProgress = false;
	for (auto& it : windowsProgress)
	{
		if (it.second == true)
		{
			windowsInProgress = true;
			break;
		}
	}

	if (EditorUI::IsAnyMouseDown())
	{
		if (!isWindowInProgress(windowName) && !windowsInProgress)
		{
			if (EditorUI::IsMouseDown(ImGuiMouseButton_Left))
				workingMouseButton = MouseCommandsButtonsEnum::LEFT;
			else if (EditorUI::IsMouseDown(ImGuiMouseButton_Right))
				workingMouseButton = MouseCommandsButtonsEnum::RIGHT;
			else if (EditorUI::IsMouseDown(ImGuiMouseButton_Middle))
				workingMouseButton = MouseCommandsButtonsEnum::MIDDLE;

			windowsProgress.insert_or_assign(windowName, true);
			beginPosAbsolute = ImVec2(ofGetMouseX(), ofGetMouseY());
			beginPosRelative = EditorUI::GetMousePos() - EditorUI::GetWindowPos() - EditorUI::GetCursorStartPos();
		}

		if (isWindowInProgress(windowName) && windowsInProgress)
		{
			computedCommands.insert_or_assign(windowName, computeWorkingCommands(windowName));
			return;
		}
	}

	//No actions being made, only hovering over window
	computedCommands.insert_or_assign(windowName, MouseCommandsDto{ beginPosAbsolute, actualPosAbsolute, beginPosRelative, actualPosRelative,
		MouseCommandsButtonsEnum::NONE, MouseCommandsTypesEnum::NONE, MouseCommandsStatesEnum::NONE });
}

MouseCommands::MouseCommandsDto MouseCommands::getComputedCommands(const string windowName)
{
	if (computedCommands.find(windowName) != computedCommands.end())
		return computedCommands.at(windowName);

	return MouseCommandsDto{ ImVec2(), ImVec2(), ImVec2(), ImVec2(), MouseCommandsButtonsEnum::NONE, MouseCommandsTypesEnum::NONE, MouseCommandsStatesEnum::NONE };
}

MouseCommands::MouseCommandsDto MouseCommands::computeWorkingCommands(const string windowName)
{
	//If the actual position is too near from the begin position, then we count the command as a click, else a drag.
	thresholdExceeded = VectorHelper::distance2d(beginPosAbsolute, actualPosAbsolute) >= DRAG_DISTANCE_THRESHOLD || thresholdExceeded;
	MouseCommandsTypesEnum command = thresholdExceeded ? MouseCommandsTypesEnum::DRAG : MouseCommandsTypesEnum::CLICK;
	MouseCommandsStatesEnum state = isWindowInProgress(windowName) ? MouseCommandsStatesEnum::IN_PROGRESS : MouseCommandsStatesEnum::DONE;

	return MouseCommandsDto{ beginPosAbsolute, actualPosAbsolute, beginPosRelative, actualPosRelative, workingMouseButton, command, state };
}

bool MouseCommands::isWindowInProgress(const string windowName)
{
	return windowsProgress.find(windowName) != windowsProgress.end() ? windowsProgress[windowName] : false;

}

ImGuiMouseButton MouseCommands::mapToImGuiMouseButtons(const MouseCommandsButtonsEnum mouseButton)
{
	switch (mouseButton)
	{
		case MouseCommandsButtonsEnum::LEFT: 
			return ImGuiMouseButton_Left;
		case MouseCommandsButtonsEnum::RIGHT: 
			return ImGuiMouseButton_Right;
		case MouseCommandsButtonsEnum::MIDDLE: 
			return ImGuiMouseButton_Middle;
		default: 
			return ImGuiMouseButton_Left;
	}
}
