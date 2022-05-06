#pragma once

#include "command.h"
#include "entity/components/imageComponent.h"

class ImportImageCommand : public Command
{

public:
	ImportImageCommand(void* image) : image(image) {};
	~ImportImageCommand() {};

	bool executeCommand() override {
		ImageImport* imageImportPtr = static_cast<ImageImport*>(image);

		auto fileChosen = ofSystemLoadDialog("Load an image file");

		if (fileChosen.bSuccess) 
		{
			std::string fileExtension = fileChosen.fileName.substr(fileChosen.fileName.find_last_of('.') + 1);

			if (isValidFile(fileExtension) && image != nullptr)
			{
				imageImportPtr->imagePath = fileChosen.filePath;
				return imageImportPtr->imagePtr->load(fileChosen.filePath);
			}
			else
			{
				return false;
			}
		}
	};

private:

	void* image;

	bool isValidFile(std::string& fileExtension) 
	{
		return fileExtension == "png" || fileExtension == "jpeg" || fileExtension == "jpg" || fileExtension == "gif";
	}

};

