#pragma once

#include "wcpch.h"

#include "customCursorTypesEnum.h"


class CustomCursor
{
public:
	static void setup();
	static void draw();

	static CustomCursorEnum getCursorType() { return cursorType; } const
	static void setCursorType(const CustomCursorEnum newCursorType) { cursorType = newCursorType; }

private:
	static CustomCursorEnum cursorType;
	static std::map<CustomCursorEnum, ImGuiMouseCursor_> defaultCursorImages;
	static std::map<CustomCursorEnum, ofImage> customCursorImages;
};

