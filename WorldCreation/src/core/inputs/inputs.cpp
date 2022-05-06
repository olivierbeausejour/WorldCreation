#include "inputs.h"

void Inputs::updateInputs()
{

}

std::string Inputs::getShortcut(const Action& action)
{
	std::string shortcut = "";

	switch (action) {
		case Action::IMPORT_IMAGE:
			shortcut = "CTRL+O";
			break;
	}

	return shortcut;
}