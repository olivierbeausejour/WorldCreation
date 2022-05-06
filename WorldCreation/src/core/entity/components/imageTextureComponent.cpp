#include "imageTextureComponent.h"

bool ImageTextureComponent::update(const bool forceUpdate)
{
	if (textureToUpdate || forceUpdate)
	{
		setTexture(image != nullptr ? new ofTexture(image->getTexture()) : nullptr);

		textureToUpdate = false;

		return true;
	}

	return false;
}
