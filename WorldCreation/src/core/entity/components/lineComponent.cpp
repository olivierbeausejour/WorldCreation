#include "lineComponent.h"

void LineComponent::draw()
{
	ofFill();
	ofSetColor(outlineColor);
	ofPushStyle();

	// CREDITS: https://forum.openframeworks.cc/t/ofsetlinewidth-maximum-value/3751/4
	glm::highp_vec3 startPos = transform->getLocalPosition() - (size / 2);
	glm::highp_vec3 endPos = transform->getLocalPosition() + (size / 2);

	lineDrawer.drawLine(startPos, endPos, lineWidth);

	ofPopStyle();
}
