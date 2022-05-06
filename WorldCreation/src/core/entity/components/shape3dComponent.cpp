#include "shape3dComponent.h"

void Shape3dComponent::draw()
{
	shape.resetTransform();

	preDraw();

	//We need to add 180 degrees in order to flip shape on x axis. This way
	//textures are drawn upright. If this wasn't done, textures would be
	//drawn upside down. Note that this does not affect the UI.
	shape.rotate(transform->getGlobalEulerAngles().x + 180, 1, 0, 0);
	shape.rotate(transform->getGlobalEulerAngles().y, 0, 1, 0);
	shape.rotate(transform->getGlobalEulerAngles().z, 0, 0, 1);

	shape.setPosition(this->transform->getGlobalPosition());
	shape.setScale(this->transform->getGlobalScale());

	if (textureComponent != nullptr)
	{
		bool forceTextureUpdate = textureComponentUpdated || filterComponentUpdated;
		textureComponent->update(forceTextureUpdate);
		
		if (textureComponent->getTexture() != nullptr)
		{
			//Set color to white (no color), in order to override drawableComponent color property.
			//This way, we do not apply color when a texture component is used.
			ofSetColor(ofFloatColor::white);

			textureComponent->getTexture()->bind();
		}
	}

	shape.draw(renderMode);

	if (textureComponent != nullptr && textureComponent->getTexture() != nullptr)
	{
		textureComponent->getTexture()->unbind();
	}

	postDraw();

	textureComponentUpdated = false;
	filterComponentUpdated = false;
}

void Shape3dComponent::preDraw()
{
	DrawableComponent::preDraw();
	if (textureComponent == nullptr || textureComponent->getTexture() == nullptr)
		ofSetColor(this->color);
}