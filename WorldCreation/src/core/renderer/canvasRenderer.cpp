#include "canvasRenderer.h"

void CanvasRenderer::setup(Scene *scene)
{
	this->scene = scene;
	drawnEntity = nullptr;
}

void CanvasRenderer::drawInternal()
{
}

void CanvasRenderer::preDraw()
{
}

void CanvasRenderer::draw(bool isFramebufferTarget)
{
	if (isFramebufferTarget)
	{
		ofFboSettings frameBufferSettings;
		frameBufferSettings.internalformat = GL_RGBA;
		frameBufferSettings.textureTarget = GL_TEXTURE_2D;
		frameBufferSettings.width = rendererWidth;
		frameBufferSettings.height = rendererHeight;
		frameBufferSettings.useDepth = true;
		fbo.allocate(frameBufferSettings);
		fbo.begin();
		ofClear(backgroundColor);
	}

	// Must draw in reverse order
	for (int k = scene->getEntityTree()->getRootEntities().size() - 1; k >= 0; k--)
	{
		scene->getEntityTree()->getRootEntities().at(k)->drawDrawableComponent();
	}

	auto mouseCommand = MouseCommands::getComputedCommands("Canvas");
	if (mouseCommand.commandState == MouseCommandsStatesEnum::IN_PROGRESS)
	{
		if (mouseCommand.mouseButton == MouseCommandsButtonsEnum::LEFT)
		{
			switch (shapeType)
			{
			case Shape2dTypesEnum::LINE:
				CustomCursor::setCursorType(CustomCursorEnum::CROSSHAIR);
				break;
			case Shape2dTypesEnum::RECTANGLE:
				CustomCursor::setCursorType(CustomCursorEnum::CROSSHAIR_RECTANGLE);
				break;
			case Shape2dTypesEnum::SQUARE:
				CustomCursor::setCursorType(CustomCursorEnum::CROSSHAIR_SQUARE);
				break;
			case Shape2dTypesEnum::ELLIPSE:
				CustomCursor::setCursorType(CustomCursorEnum::CROSSHAIR_ELLIPSE);
				break;
			case Shape2dTypesEnum::CIRCLE:
				CustomCursor::setCursorType(CustomCursorEnum::CROSSHAIR_CIRCLE);
				break;
			case Shape2dTypesEnum::TRIANGLE:
				CustomCursor::setCursorType(CustomCursorEnum::CROSSHAIR_TRIANGLE);
				break;
			case Shape2dTypesEnum::RIGHT_TRIANGLE:
				CustomCursor::setCursorType(CustomCursorEnum::CROSSHAIR_RIGHT_TRIANGLE);
				break;
			case Shape2dTypesEnum::FLOWER:
				CustomCursor::setCursorType(CustomCursorEnum::CROSSHAIR_FLOWER);
				break;
			case Shape2dTypesEnum::VERTICAL_ARROW:
				CustomCursor::setCursorType(CustomCursorEnum::CROSSHAIR_VERTICAL_ARROW);
				break;
			case Shape2dTypesEnum::HORIZONTAL_ARROW:
				CustomCursor::setCursorType(CustomCursorEnum::CROSSHAIR_HORIZONTAL_ARROW);
				break;
			default:
				CustomCursor::setCursorType(CustomCursorEnum::CROSSHAIR);
				break;
			}

			if (shapeType != Shape2dTypesEnum::BEZIER_CURVE)
			{
				selectedPointIdx = -1;
				selectedCurve = nullptr;

				if (drawnEntity != nullptr)
				{
					BezierCurveComponent* possibleCurve = dynamic_cast<BezierCurveComponent*>(drawnEntity->getDrawableComponent());

					if (possibleCurve != nullptr)
						drawingInProgress = false;
				}
			}

			ofVec2f diffPos = mouseCommand.endPosRelative - mouseCommand.beginPosRelative;
			float distance = VectorHelper::distance2d(mouseCommand.endPosRelative, mouseCommand.beginPosRelative);

			if (!drawingInProgress)
			{
				Shape2dComponent *shape2dComponent;

				switch (shapeType)
				{
				case Shape2dTypesEnum::LINE:
					shape2dComponent = new LineComponent();
					break;
				case Shape2dTypesEnum::RECTANGLE:
					shape2dComponent = new RectangleComponent();
					break;
				case Shape2dTypesEnum::SQUARE:
					shape2dComponent = new SquareComponent();
					break;
				case Shape2dTypesEnum::ELLIPSE:
					shape2dComponent = new EllipseComponent();
					break;
				case Shape2dTypesEnum::CIRCLE:
					shape2dComponent = new CircleComponent();
					break;
				case Shape2dTypesEnum::TRIANGLE:
					shape2dComponent = new TriangleComponent();
					break;
				case Shape2dTypesEnum::RIGHT_TRIANGLE:
					shape2dComponent = new RightTriangleComponent();
					break;
				case Shape2dTypesEnum::FLOWER:
					shape2dComponent = new FlowerShapeComponent();
					break;
				case Shape2dTypesEnum::VERTICAL_ARROW:
					shape2dComponent = new VerticalArrowShapeComponent();
					break;
				case Shape2dTypesEnum::HORIZONTAL_ARROW:
					shape2dComponent = new HorizontalArrowShapeComponent();
					break;
				case Shape2dTypesEnum::BEZIER_CURVE:
					if (selectedPointIdx == -1 && !newCurve)
					{
						newCurve = true;
						shape2dComponent = new BezierCurveComponent();
					}
					else
						newCurve = false;
					break;
				default:
					shape2dComponent = new LineComponent();
					break;
				}

				if (newCurve || shapeType != Shape2dTypesEnum::BEZIER_CURVE)
				{
					shape2dComponent->setColor(fillColor);
					shape2dComponent->setOutlineColor(outlineColor);
					shape2dComponent->setLineWidth(lineWidth);

					drawnEntity = scene->addEntity("");
					drawnEntity->setDrawableComponent(shape2dComponent);
				}

				drawingInProgress = true;
			}

			if (drawingInProgress)
			{
				switch (shapeType)
				{
				default:
				{
					ofVec2f shapePos2d = glm::vec3(mouseCommand.beginPosRelative.x + (diffPos.x / 2), mouseCommand.beginPosRelative.y + (diffPos.y / 2), 0);
					drawnEntity->getTransform()->setLocalPosition(glm::vec3(shapePos2d.x, shapePos2d.y, 0));
					drawnEntity->getDrawableComponent()->setSize(glm::vec3(diffPos.x, diffPos.y, 0));
					break;
				}
				case Shape2dTypesEnum::SQUARE:
				case Shape2dTypesEnum::CIRCLE:
				{
					float smallestSide = MathHelper::closestToZero(diffPos.x, diffPos.y);
					float sideX = MathHelper::getSameSignValue(diffPos.x, smallestSide);
					float sideY = MathHelper::getSameSignValue(diffPos.y, smallestSide);
					ofVec2f shapePos2d = glm::vec3(mouseCommand.beginPosRelative.x + (sideX / 2), mouseCommand.beginPosRelative.y + (sideY / 2), 0);
					drawnEntity->getTransform()->setLocalPosition(glm::vec3(shapePos2d.x, shapePos2d.y, 0));
					drawnEntity->getDrawableComponent()->setSize(glm::vec3(sideX, sideY, 0));
					break;
				}
				case Shape2dTypesEnum::BEZIER_CURVE:
				{
					ofVec2f shapePos2d = glm::vec3(mouseCommand.beginPosRelative.x + (diffPos.x / 2), mouseCommand.beginPosRelative.y + (diffPos.y / 2), 0);
					BezierCurveComponent* curve = getCurveFromPoint(shapePos2d.x, shapePos2d.y);

					if (curve == nullptr) break;

					if (mouseCommand.commandType == MouseCommandsTypesEnum::CLICK)
					{
						selectedCurve = curve;
						selectedPointIdx = curve->getSelectedPoint(shapePos2d.x, shapePos2d.y);
					}
					
					if (selectedPointIdx != -1 && selectedCurve != nullptr)
						selectedCurve->updateControlPoint(selectedPointIdx, shapePos2d.x, shapePos2d.y);					
					break;
				}
				}
			}
		}
	}
	else if (mouseCommand.commandState == MouseCommandsStatesEnum::DONE)
	{
		if (selectedPointIdx == -1)
		{
			drawingInProgress = false;
			CustomCursor::setCursorType(CustomCursorEnum::DEFAULT);
		}
	}
	else
	{
		CustomCursor::setCursorType(CustomCursorEnum::DEFAULT);
	}

	if (isFramebufferTarget)
	{
		fbo.end();
	}
}

BezierCurveComponent* CanvasRenderer::getCurveFromPoint(float pointX, float pointY)
{
	std::vector<BezierCurveComponent*> curves = getCurves();
	for (BezierCurveComponent* curve : curves)
	{
		int pointIdx = curve->getSelectedPoint(pointX, pointY);
		if (pointIdx != -1)
			return curve;
	}

	return nullptr;
}

std::vector<BezierCurveComponent*> CanvasRenderer::getCurves()
{
	std::vector<BezierCurveComponent*> curves;

	for (int i = 0; i < scene->getEntityTree()->getRootEntities().size(); i++)
	{
		BezierCurveComponent* curve = dynamic_cast<BezierCurveComponent*>(scene->getEntityTree()->getRootEntities().at(i)->getDrawableComponent());

		if (curve != nullptr)
			curves.push_back(curve);
	}

	return curves;
}

void CanvasRenderer::postDraw(bool isFrameBufferTarget)
{
}
