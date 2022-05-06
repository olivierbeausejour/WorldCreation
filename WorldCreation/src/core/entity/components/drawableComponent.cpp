#include "drawableComponent.h"

void DrawableComponent::preDraw()
{
	ofPushStyle();
	ofPushMatrix();
	ofSetColor(this->color);
}

void DrawableComponent::postDraw()
{
	ofPopMatrix();
	ofPopStyle();
}
