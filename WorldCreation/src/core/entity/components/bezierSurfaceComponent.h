#pragma once

#include "wcpch.h"

#include "helpers/mathHelper.h"
#include "shape3dComponent.h"

class BezierSurfaceComponent : public Shape3dComponent
{

public:
    BezierSurfaceComponent();
    ~BezierSurfaceComponent(){};

    void draw() override;

    vector<vector<glm::vec3>> getControlPoints()
    {
        return controlPoints;
    }
    void setControlPoints(vector<vector<glm::vec3>> points)
    {
        controlPoints = points;
        resetSurface();
    }

private:
    vector<vector<glm::vec3>> controlPoints;
    glm::vec3 interpolateBezier(float u, float v);
    void resetSurface();
};