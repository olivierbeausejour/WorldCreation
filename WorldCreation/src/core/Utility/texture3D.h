// Inspired from https://github.com/timscaffidi/ofxVolumetrics
#pragma once
#include "wcpch.h"

#include "ofTexture.h"


class Texture3D
{

public:

	Texture3D() = default;
	~Texture3D() = default;

	bool loadTextures(vector<string> textures, int width, int height, int depth, GLenum glFormat);

	int getTextureID() { return id; }

private:

	float width, height, depth;
	int glType;
	int glTypeInternal;
	int pixelType;
	int id;
};