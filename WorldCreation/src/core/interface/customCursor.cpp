#include "customCursor.h"

CustomCursorEnum CustomCursor::cursorType;
std::map<CustomCursorEnum, ImGuiMouseCursor_> CustomCursor::defaultCursorImages;
std::map<CustomCursorEnum, ofImage> CustomCursor::customCursorImages;

static constexpr int CURSOR_SIZE = 24;

void CustomCursor::setup()
{
	cursorType = CustomCursorEnum::DEFAULT;

	defaultCursorImages.insert(std::pair<CustomCursorEnum, ImGuiMouseCursor_>(CustomCursorEnum::DEFAULT, ImGuiMouseCursor_Arrow));

	customCursorImages.insert(std::pair<CustomCursorEnum, ofImage>(CustomCursorEnum::HAND, ofImage("editor/icons/cursor_hand.png")));
	customCursorImages.insert(std::pair<CustomCursorEnum, ofImage>(CustomCursorEnum::QUAD_ARROW, ofImage("editor/icons/cursor_quad_arrow.png")));
	customCursorImages.insert(std::pair<CustomCursorEnum, ofImage>(CustomCursorEnum::ZOOM_IN, ofImage("editor/icons/cursor_zoom_in.png")));
	customCursorImages.insert(std::pair<CustomCursorEnum, ofImage>(CustomCursorEnum::ZOOM_OUT, ofImage("editor/icons/cursor_zoom_out.png")));
	customCursorImages.insert(std::pair<CustomCursorEnum, ofImage>(CustomCursorEnum::EYE, ofImage("editor/icons/cursor_eye.png")));
	customCursorImages.insert(std::pair<CustomCursorEnum, ofImage>(CustomCursorEnum::CROSSHAIR, ofImage("editor/icons/cursor_crosshair.png")));
	customCursorImages.insert(std::pair<CustomCursorEnum, ofImage>(CustomCursorEnum::CROSSHAIR_RECTANGLE, ofImage("editor/icons/cursor_crosshair_rectangle.png")));
	customCursorImages.insert(std::pair<CustomCursorEnum, ofImage>(CustomCursorEnum::CROSSHAIR_SQUARE, ofImage("editor/icons/cursor_crosshair_square.png")));
	customCursorImages.insert(std::pair<CustomCursorEnum, ofImage>(CustomCursorEnum::CROSSHAIR_ELLIPSE, ofImage("editor/icons/cursor_crosshair_ellipse.png")));
	customCursorImages.insert(std::pair<CustomCursorEnum, ofImage>(CustomCursorEnum::CROSSHAIR_CIRCLE, ofImage("editor/icons/cursor_crosshair_circle.png")));
	customCursorImages.insert(std::pair<CustomCursorEnum, ofImage>(CustomCursorEnum::CROSSHAIR_TRIANGLE, ofImage("editor/icons/cursor_crosshair_triangle.png")));
	customCursorImages.insert(std::pair<CustomCursorEnum, ofImage>(CustomCursorEnum::CROSSHAIR_RIGHT_TRIANGLE, ofImage("editor/icons/cursor_crosshair_right_triangle.png")));
	customCursorImages.insert(std::pair<CustomCursorEnum, ofImage>(CustomCursorEnum::CROSSHAIR_FLOWER, ofImage("editor/icons/cursor_crosshair_flower.png")));
	customCursorImages.insert(std::pair<CustomCursorEnum, ofImage>(CustomCursorEnum::CROSSHAIR_VERTICAL_ARROW, ofImage("editor/icons/cursor_crosshair_vertical_arrow.png")));
	customCursorImages.insert(std::pair<CustomCursorEnum, ofImage>(CustomCursorEnum::CROSSHAIR_HORIZONTAL_ARROW, ofImage("editor/icons/cursor_crosshair_horizontal_arrow.png")));
}

void CustomCursor::draw()
{
	if (defaultCursorImages.find(cursorType) != defaultCursorImages.end())
	{
		EditorUI::SetMouseCursor(defaultCursorImages.at(cursorType));
	}
	else if (customCursorImages.find(cursorType) != customCursorImages.end())
	{
		EditorUI::SetMouseCursor(ImGuiMouseCursor_None);
		customCursorImages.at(cursorType).draw(ofGetMouseX() - (CURSOR_SIZE / 2), ofGetMouseY() - (CURSOR_SIZE / 2), CURSOR_SIZE, CURSOR_SIZE);
	}
	else
	{
		EditorUI::SetMouseCursor(ImGuiMouseCursor_Arrow);
	}
}
