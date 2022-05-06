#pragma once

#include "command.h"
#include "importImageCommand.hpp"
#include "createSceneViewportCommand.h"
#include "importModelCommand.hpp"
#include "inputs/actions.h"
#include "renderer/canvasRenderer.h"
#include "interface/interface.h"

class CommandFactory {

public:

	CommandFactory() {};
	~CommandFactory() {};

	Command* getCommand(const Action& action, void* object)
	{
		switch (action) 
		{
			case Action::IMPORT_IMAGE:
				return new ImportImageCommand(object);
				break;
			case Action::CREATE_SCENE_VIEWPORT:
				return new CreateSceneViewportCommand(object); 
				break;
			case Action::IMPORT_MODEL:
				return new ImportModelCommand(object);
				break;
		}

		return nullptr;
	}

};