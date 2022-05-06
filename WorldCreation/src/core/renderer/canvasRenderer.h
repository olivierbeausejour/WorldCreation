#pragma once

#include "wcpch.h"

#include "abstractRenderer.h"

#include "interface/customCursor.h"

#include "inputs/mouseCommands.h"

#include "helpers/vectorHelper.h"
#include "helpers/mathHelper.h"

#include "entity/entity.h"
#include "entity/components/shape2dTypesEnum.h"
#include "entity/components/lineComponent.h"
#include "entity/components/rectangleComponent.h"
#include "entity/components/squareComponent.h"
#include "entity/components/ellipseComponent.h"
#include "entity/components/circleComponent.h"
#include "entity/components/triangleComponent.h"
#include "entity/components/rightTriangleComponent.h"
#include "entity/components/flowerShapeComponent.h"
#include "entity/components/verticalArrowShapeComponent.h"
#include "entity/components/horizontalArrowShapeComponent.h"
#include "entity/components/bezierCurveComponent.h"
#include "scene/scene.h"

class CanvasRenderer : public AbstractRenderer
{

public:
	CanvasRenderer() : AbstractRenderer(1000, 1080){};
	~CanvasRenderer()
	{
		// TODO: DELETE DRAWING COMPONENTS IN ENTITY DESTRUCTOR, WHEN SCENE IMPLEMENTATION IS DONE IN SCENE RENDERER
		for (auto *entity : scene->getEntityTree()->getRootEntities())
		{
			delete entity->getDrawableComponent();
		}
	}

	void setup(Scene *scene);
	void drawInternal();
	void preDraw();
	void draw(bool isFramebufferTarget = true);
	void postDraw(bool isFramebufferTarget = true);

	void setPrimitiveToDraw(const Shape2dTypesEnum shape) { shapeType = shape; };
	void setFillColor(const ofFloatColor color) { fillColor = color; };
	void setOutlineColor(const ofFloatColor color) { outlineColor = color; };
	void setBackgroundColor(const ofFloatColor color) { backgroundColor = color; };
	void setLineWidth(const int width) { lineWidth = width; };
	void addImage(Entity *image) { scene->getEntityTree()->addEntity(*image); };

private:
	BezierCurveComponent* getCurveFromPoint(float pointX, float pointY);
	std::vector<BezierCurveComponent*> getCurves();

	Shape2dTypesEnum shapeType;
	ofFloatColor fillColor = ofFloatColor::red;
	ofFloatColor outlineColor = ofFloatColor::black;
	ofFloatColor backgroundColor = ofFloatColor::white;
	int lineWidth = 1;

	int selectedPointIdx = -1;
	BezierCurveComponent* selectedCurve = nullptr;
	bool newCurve = false;

	bool drawingInProgress = false;
	Entity *drawnEntity;

	Scene *scene;
};