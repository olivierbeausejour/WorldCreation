#include "sceneRenderer.h"

void SceneRenderer::setup(Scene *scene)
{
	ofEnableDepthTest();

	model.setScale(0.1f, 0.1f, 0.1f);

	ofMesh *boundingBoxMesh = delimitationBox.getMeshPtr();
	boundingBoxMesh->setMode(OF_PRIMITIVE_LINES);
	boundingBoxMesh->enableIndices();

	boundingBoxMesh->addVertex(ofVec3f(1, 1, 1));
	boundingBoxMesh->addVertex(ofVec3f(1, -1, 1));
	boundingBoxMesh->addVertex(ofVec3f(-1, 1, 1));
	boundingBoxMesh->addVertex(ofVec3f(1, 1, -1));
	boundingBoxMesh->addVertex(ofVec3f(-1, -1, 1));
	boundingBoxMesh->addVertex(ofVec3f(-1, 1, -1));
	boundingBoxMesh->addVertex(ofVec3f(-1, -1, -1));
	boundingBoxMesh->addVertex(ofVec3f(1, -1, -1));

	boundingBoxMesh->addIndex(0);
	boundingBoxMesh->addIndex(1);
	boundingBoxMesh->addIndex(0);
	boundingBoxMesh->addIndex(2);
	boundingBoxMesh->addIndex(0);
	boundingBoxMesh->addIndex(3);
	boundingBoxMesh->addIndex(2);
	boundingBoxMesh->addIndex(4);
	boundingBoxMesh->addIndex(2);
	boundingBoxMesh->addIndex(5);
	boundingBoxMesh->addIndex(1);
	boundingBoxMesh->addIndex(4);
	boundingBoxMesh->addIndex(1);
	boundingBoxMesh->addIndex(7);
	boundingBoxMesh->addIndex(6);
	boundingBoxMesh->addIndex(7);
	boundingBoxMesh->addIndex(6);
	boundingBoxMesh->addIndex(5);
	boundingBoxMesh->addIndex(6);
	boundingBoxMesh->addIndex(4);
	boundingBoxMesh->addIndex(3);
	boundingBoxMesh->addIndex(7);
	boundingBoxMesh->addIndex(3);
	boundingBoxMesh->addIndex(5);

	this->scene = scene;

	worldComponent = new WorldComponent();
	world.setDrawableComponent(worldComponent);
	worldComponent->setup();
	scene->getSceneEnvironnement()->setup();
}

void SceneRenderer::preDraw()
{
	ofClear(0, 255);
	glEnable(GL_DEPTH_TEST);
	editorCamera->begin();
}

void SceneRenderer::drawInternal()
{
	world.drawDrawableComponent();

	ofEnableLighting();

	for (auto entity = scene->getEntityTree()->begin(); entity != scene->getEntityTree()->end(); ++entity)
	{
		if (entity->getDrawableComponent() != nullptr)
		{
			if (dynamic_cast<LightComponent*>(entity->getDrawableComponent()) == nullptr)
			{
				ofShader* shader = nullptr;

				if (Shape3dComponent* shape3D = dynamic_cast<Shape3dComponent*>(entity->getDrawableComponent()))
				{
					if (shape3D->getFilterComponent() != nullptr)
					{
						shader = shape3D->getFilterComponent()->getShader(lightingRenderer.getLightingModel());
					}
				}

				if (!scene->getSceneEnvironnement()->getCubemapLoaded() 
					|| (dynamic_cast<MetallicBallComponent*>(entity->getDrawableComponent()) == nullptr 
					&& dynamic_cast<BezierSurfaceComponent*>(entity->getDrawableComponent()) == nullptr))
				{
					lightingRenderer.updateLightingShader(shader, 
													  entity->getDrawableComponent(), 
													  scene->getLightEntities(),
													  editorCamera->getGlobalPosition());

					drawEntityDrawableComponent(entity->getDrawableComponent());
					lightingRenderer.end(shader);
				}
				else
				{
					drawEntityDrawableComponent(entity->getDrawableComponent());
				}
			}
		}
	}

	ofDisableLighting();

	drawLights();
	drawDelimitation();
	drawCubemap();
}

void SceneRenderer::drawLights()
{
	auto lights = scene->getLightEntities();
	for (auto entity : lights)
	{
		entity->drawDrawableComponent();
	}
}

void SceneRenderer::drawEntityDrawableComponent(DrawableComponent* drawableComponent)
{
	if (drawableComponent == nullptr)
		return;

	MetallicBallComponent *metallicBallComponent = dynamic_cast<MetallicBallComponent *>(drawableComponent);
	if (metallicBallComponent != nullptr)
	{
		const bool cubemapLoaded = scene->getSceneEnvironnement()->getCubemapLoaded();
		const int cubemapNbMipLevels = scene->getSceneEnvironnement()->getCubemapNbMipLevels();
		const unsigned int cubemapTextureID = cubemapLoaded ? scene->getSceneEnvironnement()->getCubemapTextureID() : 0;
		metallicBallComponent->draw(editorCamera, cubemapTextureID, cubemapNbMipLevels, cubemapLoaded);
	}
	else
		drawableComponent->draw();
}

void SceneRenderer::draw(EditorCamera *editorCamera, bool isFramebufferTarget)
{
	if (editorCamera == nullptr)
		return;

	this->editorCamera = editorCamera;

	if (isFramebufferTarget)
	{
		ofFboSettings frameBufferSettings;
		frameBufferSettings.internalformat = GL_RGBA;
		frameBufferSettings.textureTarget = GL_TEXTURE_2D;
		frameBufferSettings.width = rendererWidth;
		frameBufferSettings.height = rendererHeight;
		frameBufferSettings.useDepth = true;
		frameBufferSettings.useStencil = true;
		frameBufferSettings.depthStencilInternalFormat = GL_DEPTH_COMPONENT; 
		frameBufferSettings.depthStencilAsTexture = true;
		fbo.allocate(frameBufferSettings);
		preProcessingFbo.allocate(frameBufferSettings);

		preProcessingFbo.begin();
		ofClear(255, 255);
	}

	preDraw();

	drawInternal();

	postDraw(isFramebufferTarget);
}

void SceneRenderer::postDraw(bool isFramebufferTarget)
{
	editorCamera->end();

	drawVolumetricClouds();
	if (isFramebufferTarget)
	{
		preProcessingFbo.end();

		fbo.begin();
		postProcessingRenderer.draw(&preProcessingFbo);
		fbo.end();
	}
}

void SceneRenderer::drawDelimitation()
{
	if (Interface::getSelectedEntity() == nullptr ||
		Interface::getSelectedEntity()->getDrawableComponent() == nullptr)
	{
		return;
	}

	delimitationBox.resetTransform();

	delimitationBox.rotate(Interface::getSelectedEntity()->getTransform()->getGlobalEulerAngles().x, 1, 0, 0);
	delimitationBox.rotate(Interface::getSelectedEntity()->getTransform()->getGlobalEulerAngles().y, 0, 1, 0);
	delimitationBox.rotate(Interface::getSelectedEntity()->getTransform()->getGlobalEulerAngles().z, 0, 0, 1);

	delimitationBox.setPosition(Interface::getSelectedEntity()->getTransform()->getGlobalPosition());

	delimitationBox.setScale(
		Interface::getSelectedEntity()->getTransform()->getGlobalScale() *
		Interface::getSelectedEntity()->getDrawableComponent()->getOrientedBoundingBox());

	ofPushStyle();

	ofSetColor(
		255.0f - (Interface::getSelectedEntity()->getDrawableComponent()->getColor().r * 255.0f),
		255.0f - (Interface::getSelectedEntity()->getDrawableComponent()->getColor().g * 255.0f),
		255.0f - (Interface::getSelectedEntity()->getDrawableComponent()->getColor().b * 255.0f));

	delimitationBox.draw();

	ofPopStyle();
}

void SceneRenderer::drawCubemap()
{
	if (!scene->getSceneEnvironnement()->getCubemapLoaded())
		return;

	glDepthFunc(GL_LEQUAL); // so we only draw to pixel that have not been drawn to
	glm::mat4 view = glm::mat4(glm::mat3(editorCamera->getModelViewMatrix())); // remove translation from the view matrix

	ofShader *cubemapShader = scene->getSceneEnvironnement()->getCubemapShader();
	cubemapShader->begin();
	cubemapShader->setUniformMatrix4f("view", view);

	// cubemap
	unsigned int cubemapTextureID = scene->getSceneEnvironnement()->getCubemapTextureID();
	unsigned int cubemapVAO = scene->getSceneEnvironnement()->getCubemapVAO();
	glBindVertexArray(cubemapVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default

	cubemapShader->end();
}

void SceneRenderer::drawVolumetricClouds()
{
	if(!scene->getSceneEnvironnement()->getVolumetricCloudActive()) return;

	ofShader* volumetricCloudShader = scene->getSceneEnvironnement()->getVolumetricCloudShader();

	glDisable(GL_DEPTH_TEST);

	volumetricCloudShader->begin();
	// camera data
	volumetricCloudShader->setUniform1f("u_nearClip", editorCamera->getNearClip());
	volumetricCloudShader->setUniform1f("u_farClip", editorCamera->getFarClip());
	volumetricCloudShader->setUniform2f("u_resolution", {rendererWidth, rendererHeight});
	float vFOV = glm::radians(editorCamera->getFov() * 0.5f);
	glm::vec2 tanFOVby2 = {tan(vFOV), tan(glm::radians( (0.5f * rendererWidth) / (0.5f * rendererHeight / tan(vFOV))) ) };
	volumetricCloudShader->setUniform2f("u_tanFOVby2", tanFOVby2);
	volumetricCloudShader->setUniform1f("u_time", ofGetElapsedTimef());

	// camera view data
	volumetricCloudShader->setUniform3f("u_cameraWorldPosition", editorCamera->getWorldPosition());
	volumetricCloudShader->setUniformMatrix4f("u_inverseProjectionViewMatrix", glm::inverse(editorCamera->getProjectionMatrix() * editorCamera->getViewMatrix()));
	volumetricCloudShader->setUniformMatrix4f("u_projectionViewMatrix", editorCamera->getProjectionMatrix() * editorCamera->getViewMatrix());
	volumetricCloudShader->setUniformMatrix4f("u_inverseProjectionMatrix", glm::inverse(editorCamera->getProjectionMatrix()));
	volumetricCloudShader->setUniformMatrix4f("u_projectionMatrix", editorCamera->getProjectionMatrix());
	volumetricCloudShader->setUniformMatrix4f("u_inverseViewMatrix", glm::inverse(editorCamera->getViewMatrix()));
	volumetricCloudShader->setUniformMatrix4f("u_viewMatrix", editorCamera->getViewMatrix());
	volumetricCloudShader->setUniformMatrix4f("u_cameraTransformMatrix", editorCamera->getTransformationMatrix());

	// cloud noise textures
	volumetricCloudShader->setUniformTexture("u_frameBufferTexture", preProcessingFbo.getTexture(), 0);
	volumetricCloudShader->setUniformTexture("u_frameBufferDepth", preProcessingFbo.getDepthTexture(), 1);
	volumetricCloudShader->setUniformTexture("u_cloudNoiseTexture", GL_TEXTURE_3D, scene->getSceneEnvironnement()->getCloudNoiseTexture(), 2);
	volumetricCloudShader->setUniformTexture("u_blueNoiseTexture", scene->getSceneEnvironnement()->getBlueNoiseTexture(), 3);

	// cloud settings
	volumetricCloudShader->setUniform1f("u_innerVolumeRadius", scene->getSceneEnvironnement()->getWorldDiameter() * 0.5 + scene->getSceneEnvironnement()->getCloudLayerStartHeight());
	volumetricCloudShader->setUniform1f("u_outerVolumeRadius", scene->getSceneEnvironnement()->getWorldDiameter() * 0.5 + scene->getSceneEnvironnement()->getCloudLayerEndHeight());
	volumetricCloudShader->setUniform3f("u_worldOrigin", scene->getSceneEnvironnement()->getWorldOrigin());

	volumetricCloudShader->setUniform1f("u_densityMultiplier", scene->getSceneEnvironnement()->getDensityMultiplier());
	volumetricCloudShader->setUniform1f("u_densityOffset", scene->getSceneEnvironnement()->getDensityOffset());
	volumetricCloudShader->setUniform1f("u_noiseScale", scene->getSceneEnvironnement()->getNoiseScale());
	volumetricCloudShader->setUniform1f("u_detailNoiseScale", scene->getSceneEnvironnement()->getDetailNoiseScale());
	volumetricCloudShader->setUniform1f("u_detailNoiseWeight", scene->getSceneEnvironnement()->getDetailNoiseWeight());
	volumetricCloudShader->setUniform3f("u_detailWeights", scene->getSceneEnvironnement()->getDetailWeights());
	volumetricCloudShader->setUniform4f("u_shapeNoiseWeights", scene->getSceneEnvironnement()->getShapeNoiseWeights());
	volumetricCloudShader->setUniform4f("u_phaseParams", scene->getSceneEnvironnement()->getPhaseParams());

	volumetricCloudShader->setUniform3f("u_shapeOffset", scene->getSceneEnvironnement()->getShapeOffset());
	volumetricCloudShader->setUniform3f("u_detailOffset", scene->getSceneEnvironnement()->getDetailOffset());

	volumetricCloudShader->setUniform1f("u_cloudSpeed", scene->getSceneEnvironnement()->getCloudSpeed());
	
	// raymarch settings
	volumetricCloudShader->setUniform1i("u_lightmarchStepCount", scene->getSceneEnvironnement()->getLightmarchStepCount());
	volumetricCloudShader->setUniform1i("u_raymarchStepCount", scene->getSceneEnvironnement()->getRaymarchStepCount());
	volumetricCloudShader->setUniform1f("u_raymarchMinStepSize", scene->getSceneEnvironnement()->getRaymarchMinStepSize());
	volumetricCloudShader->setUniform1f("u_rayOffsetStrength", scene->getSceneEnvironnement()->getRayOffsetStrength());
	
	// light settings
	volumetricCloudShader->setUniform1f("u_lightAbsorptionThroughCloud", scene->getSceneEnvironnement()->getLightAbsorptionThroughCloud());
	volumetricCloudShader->setUniform1f("u_lightAbsorptionTowardSun", scene->getSceneEnvironnement()->getLightAbsorptionTowardSun());
	volumetricCloudShader->setUniform1f("u_darknessThreshold", scene->getSceneEnvironnement()->getDarknessThreshold());
	volumetricCloudShader->setUniform4f("u_lightColor", scene->getSceneEnvironnement()->getLightColor());
	volumetricCloudShader->setUniform3f("u_sunDir", glm::normalize(scene->getSceneEnvironnement()->getSunDir()));
		
	glBindVertexArray(scene->getSceneEnvironnement()->getQuadVAO());
	glDrawArrays(GL_TRIANGLES, 0, 6);

	volumetricCloudShader->end();
}

void SceneRenderer::updatePostProcessingShader(const float exposureFactor, const float gammaFactor, const ToneMappingTechnique mappingTechnique)
{
	postProcessingRenderer.updatePostProcessingShader(exposureFactor, gammaFactor, mappingTechnique);
}
