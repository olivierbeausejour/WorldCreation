#pragma once

#include "wcpch.h"

#include "filterComponent.h"

class InvertFilterComponent : public FilterComponent
{

public:

    InvertFilterComponent() : FilterComponent(FilterTypesEnum::INVERT, 
											  std::map<LightingModel, std::string> {
												  {LightingModel::LAMBERT, "resources/shader/lighting/lambert/lambertInvert.vert"},
												  {LightingModel::PHONG, "resources/shader/lighting/phong/phongInvert.vert"},
												  {LightingModel::BLINN_PHONG, "resources/shader/lighting/blinn_phong/blinn_phongInvert.vert"}
											  },
											  std::map<LightingModel, std::string> {
												  {LightingModel::LAMBERT, "resources/shader/lighting/lambert/lambertInvert.frag"},
												  {LightingModel::PHONG, "resources/shader/lighting/phong/phongInvert.frag"},
												  {LightingModel::BLINN_PHONG, "resources/shader/lighting/blinn_phong/blinn_phongInvert.frag"}
											  }) {};
    ~InvertFilterComponent() {};

};