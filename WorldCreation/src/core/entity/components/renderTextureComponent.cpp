#include "renderTextureComponent.h"

bool RenderTextureComponent::update(const bool forceUpdate)
{
	setTexture(source != nullptr ? new ofTexture(source->getFramebuffer().getTexture()) : nullptr);
	return true;
}
