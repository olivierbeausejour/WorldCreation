#pragma once

#include "wcpch.h"

#include "filterComponent.h"

class GrayscaleFilterComponent : public FilterComponent
{

public:

    GrayscaleFilterComponent() : FilterComponent(FilterTypesEnum::GRAYSCALE, 
												 std::map<LightingModel, std::string> 
												 {
												 	{LightingModel::LAMBERT, "resources/shader/lighting/lambert/lambertGrayscale.vert"},
												  	{LightingModel::PHONG, "resources/shader/lighting/phong/phongGrayscale.vert"},
												  	{LightingModel::BLINN_PHONG, "resources/shader/lighting/blinn_phong/blinn_phongGrayscale.vert"}
											     },
											  	 std::map<LightingModel, std::string> {
												  	{LightingModel::LAMBERT, "resources/shader/lighting/lambert/lambertGrayscale.frag"},
												  	{LightingModel::PHONG, "resources/shader/lighting/phong/phongGrayscale.frag"},
												  	{LightingModel::BLINN_PHONG, "resources/shader/lighting/blinn_phong/blinn_phongGrayscale.frag"}
											 	 }) {};
    ~GrayscaleFilterComponent() {};

};