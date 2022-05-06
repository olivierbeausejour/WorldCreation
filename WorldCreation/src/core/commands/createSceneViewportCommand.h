#pragma once

#include "command.h"

#include "interface/sceneViewport.h"

class Interface;

class CreateSceneViewportCommand : public Command
{

public:
	CreateSceneViewportCommand(void* interface) : interface(interface) {};
	~CreateSceneViewportCommand() {};

	bool executeCommand() override;

private:

	void* interface = nullptr;

};

