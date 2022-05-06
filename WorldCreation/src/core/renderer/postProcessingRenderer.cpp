
#include "postProcessingRenderer.h"

PostProcessingRenderer::PostProcessingRenderer()
{
	postProcessingShader.load("resources/shader/toneMapping/toneMapping.vert", "resources/shader/toneMapping/toneMapping.frag");
}

void PostProcessingRenderer::updatePostProcessingShader(const float exposureFactor, const float gammaFactor, const ToneMappingTechnique mappingTechnique)
{
	postProcessingShader.begin();
	postProcessingShader.setUniform1f("exposureFactor", exposureFactor);
	postProcessingShader.setUniform1f("gammaFactor", gammaFactor);
	postProcessingShader.setUniform1i("toneMappingTechnique", static_cast<int>(mappingTechnique));
	postProcessingShader.end();
}

void PostProcessingRenderer::draw(ofFbo* preProcessedSceneFbo)
{
	ofClear(40, 40, 40, 255);
	postProcessingShader.begin();
	preProcessedSceneFbo->getTexture().bind();
	scenePlane.set(preProcessedSceneFbo->getWidth(), preProcessedSceneFbo->getHeight());
	scenePlane.setScale(glm::vec3(1, -1, 1));
	scenePlane.setPosition(preProcessedSceneFbo->getWidth()/2, preProcessedSceneFbo->getHeight()/2, 0);
	scenePlane.setResolution(2, 2);
	scenePlane.draw();
	preProcessedSceneFbo->getTexture().unbind();
	postProcessingShader.end();
}