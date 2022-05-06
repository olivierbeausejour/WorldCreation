#pragma once

#include "wcpch.h"

#include "mouseCommandsButtonsEnum.h"
#include "mouseCommandsTypesEnum.h"
#include "mouseCommandsStatesEnum.h"

#include "helpers/vectorHelper.h"


class MouseCommands
{
public:
	struct MouseCommandsDto
	{
		ImVec2 beginPosAbsolute;
		ImVec2 endPosAbsolute;
		ImVec2 beginPosRelative;
		ImVec2 endPosRelative;
		MouseCommandsButtonsEnum mouseButton;
		MouseCommandsTypesEnum commandType;
		MouseCommandsStatesEnum commandState;
	};

	static void computeCommands(const string windowName);
	static MouseCommandsDto getComputedCommands(const string windowName);

private:
	static bool thresholdExceeded;
	static MouseCommandsButtonsEnum workingMouseButton;
	static ImVec2 beginPosAbsolute;
	static ImVec2 actualPosAbsolute;
	static ImVec2 beginPosRelative;
	static ImVec2 actualPosRelative;

	static map<string, MouseCommandsDto> computedCommands;
	static map<string, bool> windowsProgress;

	static MouseCommandsDto computeWorkingCommands(const string windowName);
	static bool isWindowInProgress(const string windowName);
	static ImGuiMouseButton mapToImGuiMouseButtons(const MouseCommandsButtonsEnum mouseButton);
};

