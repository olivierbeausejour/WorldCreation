#include "shape2dComponent.h"

void Shape2dComponent::draw()
{
	//Draw filling
	ofPushStyle();

	ofFill();
	ofSetColor(color);

	ofBeginShape();

	for (ofVec2f& vertex : vertices)
	{
		ofVertex(vertex);
	}

	ofEndShape();

	ofPopStyle();

	ofPushStyle();
	ofFill();
	ofSetColor(outlineColor);

	ofBeginShape();

	//Draw outline
	//MUST SET VERTICES POSITIONS IN CLOCKWISE ORDER IN ORDER TO WORK
	for (size_t k = 0; k < vertices.size(); k++)
	{
		ofVec2f vertex = vertices.at(k);

		ofVec2f previousVertex = k != 0 ? vertices.at(k - 1) : vertices.at(vertices.size() - 1);
		ofVec2f nextVertex = k != vertices.size() - 1 ? vertices.at(k + 1) : vertices.at(0);

		ofVec2f currentDirection = (vertex - previousVertex).getNormalized();
		ofVec2f nextDirection = (nextVertex - vertex).getNormalized();

		ofVec2f currentPointToIntersect = vertex + ((!MathHelper::isSameSignValue(size.x, size.y) ? 1 : -1) * (currentDirection.perpendiculared() * lineWidth));
		ofVec2f nextPointToIntersect = vertex + ((!MathHelper::isSameSignValue(size.x, size.y) ? 1 : -1) * (nextDirection.perpendiculared() * lineWidth));

		ofVec2f currentLineToIntersectStart = ofVec2f(
			currentPointToIntersect.x - (currentDirection.x * MAX_INTERSECTION_DISTANCE_CHECK),
			currentPointToIntersect.y - (currentDirection.y * MAX_INTERSECTION_DISTANCE_CHECK));
		ofVec2f currentLineToIntersectEnd = ofVec2f(
			currentPointToIntersect.x + (currentDirection.x * MAX_INTERSECTION_DISTANCE_CHECK),
			currentPointToIntersect.y + (currentDirection.y * MAX_INTERSECTION_DISTANCE_CHECK));

		ofVec2f nextLineToIntersectStart = ofVec2f(
			nextPointToIntersect.x - (nextDirection.x * MAX_INTERSECTION_DISTANCE_CHECK),
			nextPointToIntersect.y - (nextDirection.y * MAX_INTERSECTION_DISTANCE_CHECK));
		ofVec2f nextLineToIntersectEnd = ofVec2f(
			nextPointToIntersect.x + (nextDirection.x * MAX_INTERSECTION_DISTANCE_CHECK),
			nextPointToIntersect.y + (nextDirection.y * MAX_INTERSECTION_DISTANCE_CHECK));

		ofVec2f intersection;

		bool intersectionSuccessful = ofLineSegmentIntersection(currentLineToIntersectStart, currentLineToIntersectEnd, nextLineToIntersectStart, nextLineToIntersectEnd, intersection);

		if (intersectionSuccessful)
			ofVertex(intersection);
	}

	ofEndShape();

	ofPopStyle();
}

void Shape2dComponent::setSize(const glm::vec3 size)
{
	DrawableComponent::setSize(size);

	vertices.clear();
}
