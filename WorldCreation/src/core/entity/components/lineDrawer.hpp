#pragma once

#include "wcpch.h"

class LineDrawer
{
public:
    LineDrawer() {}
    ~LineDrawer() {}

    void drawLine(glm::highp_vec3 startPos, glm::highp_vec3 endPos, int lineWidth)
    {
        float angle = atan((endPos.y - startPos.y) / (endPos.x - startPos.x));
        ofPushMatrix();
        ofTranslate(startPos.x, startPos.y);
        ofRotate(ofRadToDeg(angle));
        float lineLength = (endPos.x - startPos.x) / cos(angle);
        ofDrawRectangle(0, -(lineWidth + 1) / 2, lineLength, (lineWidth + 1) / 2);
        ofPopMatrix();
    }
};