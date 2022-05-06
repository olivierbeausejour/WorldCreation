#include "createSceneViewportCommand.h"
#include "interface/interface.h"

bool CreateSceneViewportCommand::executeCommand()
{
    Interface* interfacePtr = static_cast<Interface*>(interface);
    SceneViewport* sceneViewport = new SceneViewport();

    size_t viewportNumber = 0;
    string viewportName = "";
    while (true)
    {
        viewportNumber++;

        viewportName = "Scene (" + to_string(viewportNumber) + ")";

        bool sameNameFound = false;
        for (SceneViewport* viewport : interfacePtr->getSceneViewports())
        {
            if (strcmp(viewport->getName().data(), viewportName.data()) == 0)
            {
                sameNameFound = true;
                break;
            }
        }

        if (!sameNameFound)
            break;
    }

    sceneViewport->setName(viewportName);
    sceneViewport->setCanClose(true);
    interfacePtr->addSceneViewport(sceneViewport);

    return true;
}
