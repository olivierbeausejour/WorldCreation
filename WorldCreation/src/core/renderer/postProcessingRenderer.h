#pragma once

#include "wcpch.h"

#include "interface/utilityPanels/toneMappingTechniques.h"

class PostProcessingRenderer
{

public:

	PostProcessingRenderer();
	~PostProcessingRenderer() = default;

	void draw(ofFbo* preProcessedSceneFbo);

	void updatePostProcessingShader(const float exposureFactor, const float gammaFactor, const ToneMappingTechnique mappingTechnique);

private:
	ofShader postProcessingShader;
	ofPlanePrimitive scenePlane;
};