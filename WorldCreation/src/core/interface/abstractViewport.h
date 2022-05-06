#pragma once

#include "wcpch.h"


class AbstractViewport
{

public:

	AbstractViewport(int width, int height, char* name) : 
		viewWidth(width),
		viewHeight(height), 
		name(name) ,
		canClose(false),
		isOpen(true) { };
	~AbstractViewport() { };

	virtual void draw() = 0;

	void resize(const int width, const int height) 
	{
		viewWidth = width;
		viewHeight = height;
	};

	int getViewWidth() { return viewWidth; }
	int getViewHeight() { return viewHeight; }

	void setCanClose(const bool value) { canClose = value; }
	bool getIsOpen() const { return isOpen; }

	ofFbo& getFramebuffer() { return *fbo; }
	void setFramebuffer(ofFbo* fbo) { this->fbo = fbo; }

	string getName() const { return name; }
	void setName(string name)
	{ 
		this->name = name;
	}

protected:

	int viewWidth;
	int viewHeight;
	bool canClose;
	bool isOpen;

	ofFbo* fbo;

	string name = "";
};