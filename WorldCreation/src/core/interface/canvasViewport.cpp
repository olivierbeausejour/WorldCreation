#include "canvasViewport.h"


void CanvasViewport::draw()
{
	EditorUI::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
	EditorUI::Begin(name.data(), NULL);

	ofTexture texture = fbo->getTexture();
	ImVec2 sceneViewSize = EditorUI::GetContentRegionAvail();

	viewWidth = sceneViewSize.x;
	viewHeight = sceneViewSize.y;
	EditorUI::Image((ImTextureID)(uintptr_t)texture.getTextureData().textureID, sceneViewSize);

	MouseCommands::computeCommands(name);

	EditorUI::PopStyleVar();
	EditorUI::End();
}
