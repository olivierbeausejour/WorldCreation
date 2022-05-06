#pragma once

#include "wcpch.h"

#include "scene/scene.h"


class AbstractRenderer
{

public:

	AbstractRenderer(int width, int height) : rendererWidth(width), rendererHeight(height) {};
	~AbstractRenderer() {};

	int getRendererWidth() { return rendererWidth; };
	void setRendererWidth(const int& value) { rendererWidth = value; };
	int getRendererHeight() { return rendererHeight; };
	void setRendererHeight(const int& value) { rendererHeight = value; };

	ofFbo* getFramebufferPtr() { return &fbo; };

protected:
	int rendererWidth;
	int rendererHeight;

	ofFbo fbo;
};