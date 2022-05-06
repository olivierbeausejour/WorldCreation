#include "wcpch.h"
#include "core/application/application.h"

#ifdef _DEBUG
// FOR MEMORY LEAK DETECTION
#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h>  
#include <crtdbg.h>   
#endif


int main()
{
#ifdef _DEBUG
	_CrtMemState sOld;
	_CrtMemState sNew;
	_CrtMemState sDiff;
	_CrtMemCheckpoint(&sOld); //take a snapshot
#endif

	ofRunApp(new Application());

}
