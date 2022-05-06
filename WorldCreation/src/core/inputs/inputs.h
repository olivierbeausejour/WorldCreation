#pragma once

#include <string>
#include "actions.h"

class Inputs
{

public:
	Inputs() {};
	~Inputs() {};

	void updateInputs();
	static std::string getShortcut(const Action& action);

private:

};
