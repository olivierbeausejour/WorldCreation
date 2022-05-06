#pragma once

#include "entity/components/drawableComponent.h"

class WorldComponent : public DrawableComponent
{

public:
	WorldComponent();
	~WorldComponent() { }

	void setup();
private:
	const float MAP_SCALE = 600;

	float width = 960;
	float height = 540;
	int octaves = 7;
	float startAmplitude = 1.0f;
	float startFrequency = 1.0f;
	float amplitudeFactor = 0.5f;
	float frequencyFactor = 2.0f;
	ofImage heightMap;

	ofMesh* worldMesh;
	of3dPrimitive world;
	ofShader worldShader;

	void generateHeightMap();
	void generateWorldMesh();
	void draw() override;
};