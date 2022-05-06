#include "gradientTextureComponent.h"

bool GradientTextureComponent::update(const bool forceUpdate)
{
    if (textureToUpdate || forceUpdate)
    {
        for (int x = 0; x < GRADIENT_SIZE; x++)
        {
            for (int y = 0; y < GRADIENT_SIZE; y++)
            {
                float lerpValue = (type == GradientTypesEnum::HORIZONTAL ? static_cast<float>(x) : static_cast<float>(y)) / static_cast<float>(GRADIENT_SIZE);
                image.setColor(x, y, beginColor.getLerped(endColor, lerpValue));
            }
        }

        image.update();
        setTexture(new ofTexture(image.getTexture()));
        textureToUpdate = false;
        
        return true;
    }

    return false;
}