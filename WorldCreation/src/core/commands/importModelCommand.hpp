#pragma once

#include "wcpch.h"

#include "command.h"


class ImportModelCommand : public Command
{

public:
	ImportModelCommand(void* model) : model(model) {};
	~ImportModelCommand() {};

	bool executeCommand() override 
	{
		string filePath = "";
		ModelImport* modelImportPtr = static_cast<ModelImport*>(model);
		if (modelImportPtr->modelName == "")
		{
			auto fileChosen = ofSystemLoadDialog("Load a 3d model file");

			if (fileChosen.bSuccess) 
			{
				std::string fileExtension = fileChosen.fileName.substr(fileChosen.fileName.find_last_of('.') + 1);

				if (isValidFile(fileExtension) && model != nullptr)
				{
					filePath = fileChosen.getPath();
					modelImportPtr->modelName = fileChosen.fileName;
				}
			}
		}
		else
		{
			filePath = modelImportPtr->modelName;
			modelImportPtr->modelName = modelImportPtr->modelName.substr(modelImportPtr->modelName.find_last_of("/\\") + 1);
		}

		return modelImportPtr->modelPtr->loadModel(filePath);
	};

private:

	void* model;

	bool isValidFile(std::string& fileExtension)
	{
		return fileExtension == "dae" || fileExtension == "ply" || fileExtension == "obj" || fileExtension == "fbx";
	}
};

