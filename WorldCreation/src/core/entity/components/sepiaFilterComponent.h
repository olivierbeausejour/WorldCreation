#pragma once

#include "wcpch.h"

#include "filterComponent.h"

class SepiaFilterComponent : public FilterComponent
{

public:

    SepiaFilterComponent() : FilterComponent(FilterTypesEnum::SEPIA, 
							 				 std::map<LightingModel, std::string> 
											 {
											 	{LightingModel::LAMBERT, "resources/shader/lighting/lambert/lambertSepia.vert"},
												{LightingModel::PHONG, "resources/shader/lighting/phong/phongSepia.vert"},
												{LightingModel::BLINN_PHONG, "resources/shader/lighting/blinn_phong/blinn_phongSepia.vert"}
											 },
											 std::map<LightingModel, std::string> 
											 {
												{LightingModel::LAMBERT, "resources/shader/lighting/lambert/lambertSepia.frag"},
												{LightingModel::PHONG, "resources/shader/lighting/phong/phongSepia.frag"},
												{LightingModel::BLINN_PHONG, "resources/shader/lighting/blinn_phong/blinn_phongSepia.frag"}
											 }) {};
    ~SepiaFilterComponent() {};

};