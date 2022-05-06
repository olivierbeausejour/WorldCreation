#pragma once

#include <string>

class Command
{

public:
	virtual bool executeCommand() = 0;
	virtual void undo() {};
};