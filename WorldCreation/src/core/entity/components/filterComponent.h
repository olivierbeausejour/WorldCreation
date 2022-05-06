#pragma once

#include "wcpch.h"

#include "filterTypesEnum.h"
#include "lightingModels.h"

class FilterComponent
{

public:

    FilterComponent(const FilterTypesEnum type, std::map<LightingModel, string> vertShaderPaths, std::map<LightingModel, string> fragShaderPaths) : 
        type(type),
        vertShaderPaths(vertShaderPaths),
        fragShaderPaths(fragShaderPaths)
    {
		for (auto path : vertShaderPaths)
		{
			filterShaders[path.first].load(path.second, fragShaderPaths[path.first]);
		}
    };

    //Needed virtual destructor in order to downcast dynamically.
    virtual ~FilterComponent() {};

    FilterTypesEnum getType() const { return type; }

    string getVertShaderPath(LightingModel lightingModel) { return vertShaderPaths[lightingModel]; }

    string getFragShaderPath(LightingModel lightingModel) { return fragShaderPaths[lightingModel]; }

	ofShader* getShader(LightingModel lightingModel) { return &filterShaders[lightingModel]; }

    void begin(const ofTexture* texture, LightingModel lightingModel) 
    { 
        filterShaders[lightingModel].begin(); 
        filterShaders[lightingModel].setUniformTexture("tex", *texture, 1);
    }
    void endFilter(LightingModel lightingModel) { filterShaders[lightingModel].end(); }

protected:

    FilterTypesEnum type;
    std::map<LightingModel, string> vertShaderPaths;
    std::map<LightingModel, string> fragShaderPaths;
    std::map<LightingModel, ofShader> filterShaders;

};