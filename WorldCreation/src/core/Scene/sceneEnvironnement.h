#pragma once

#include "wcpch.h"
#include "ofImage.h"
#include "core/Utility/texture3D.h"


class SceneEnvironnement
{

public:

	SceneEnvironnement() : 
		cubemapLoaded(false),
		nbMipLevels(0),
		volumetricCloudActive(false), 
		quadVAO(0), 
		cloudLayerStartHeight(200.0f),
		cloudLayerEndHeight(400.0f),
		worldDiameter(10000000.0f),
		worldOrigin(0.0f, 5000000.0f, 0.0f),
		densityMultiplier(4.5f),
		densityOffset(5.0f),
		noiseScale(1.0f),
		detailNoiseScale(10.0f),
		detailNoiseWeight(4.0f),
		detailWeights(0.95f, 0.42f, 0.51f),
		shapeNoiseWeights(1.0f, 0.5f, 1.0f, 0.0f),
		phaseParams(0.5f, 0.5f, 0.5f, 0.7f),
		shapeOffset(0.0f),
		detailOffset(1.0f),
		cloudSpeed(5.0f),
		lightmarchStepCount(6.0f),
		raymarchStepCount(128.0f),
		raymarchMinStepSize(0.5f),
		rayOffsetStrength(50.0f),
		lightAbsorptionThroughCloud(0.6f),
		lightAbsorptionTowardSun(1.6f),
		darknessThreshold(-0.5f),
		lightColor(0.9f),
		sunDir(37.0f, -55.0f, 68.0f) {}
	~SceneEnvironnement() = default;
	void setup();
	void setupShaders();
	void setupVolumeTextures();

	// cubemap
	bool loadCubemapTextures(vector<string> texturesPath);
	int getCubemapNbMipLevels() { return nbMipLevels; }
	unsigned int getCubemapTextureID() { return cubemapTextureID; }
	unsigned int getCubemapVAO() { return cubemapVAO; }
	unsigned int getCubemapVBO() { return cubemapVBO; }
	ofShader* getCubemapShader() { return &cubemapShader; }
	bool getCubemapLoaded() { return cubemapLoaded; }

	// volumetric clouds
	bool getVolumetricCloudActive() { return volumetricCloudActive; }
	void setVolumetricCloudActive(bool val) { this->volumetricCloudActive = val; }
	unsigned int getQuadVAO() { return quadVAO; }
	unsigned int getQuadVBO() { return quadVBO; }
	ofShader* getVolumetricCloudShader() { return &volumetricCloudShader; }
	int getCloudNoiseTexture() { return cloudNoiseTexture.getTextureID(); }
	ofTexture getBlueNoiseTexture() { return blueNoiseTexture.getTexture(); }

	// Cloud settings
	float getCloudLayerStartHeight() { return cloudLayerStartHeight; }
	float* getCloudLayerStartHeightPtr() { return &cloudLayerStartHeight; }
	float getCloudLayerEndHeight() { return cloudLayerEndHeight; }
	float* getCloudLayerEndHeightPtr() { return &cloudLayerEndHeight; }
	float getWorldDiameter() { return worldDiameter; }
	float* getWorldDiameterPtr() { return &worldDiameter; }
	glm::vec3 getWorldOrigin() { return worldOrigin; }
	void setWorldOrigin(glm::vec3 worldOrigin) { this->worldOrigin = worldOrigin; }
	glm::vec3* getWorldOriginPtr() { return &worldOrigin; }

	float getDensityMultiplier() { return densityMultiplier; }
	float* getDensityMultiplierPtr() { return &densityMultiplier; }
	float getDensityOffset() { return densityOffset; }
	float* getDensityOffsetPtr() { return &densityOffset; }
	float getNoiseScale() { return noiseScale; }
	float* getNoiseScalePtr() { return &noiseScale; }
	float getDetailNoiseScale() { return detailNoiseScale; }
	float* getDetailNoiseScalePtr() { return &detailNoiseScale; }
	float getDetailNoiseWeight() { return detailNoiseWeight; }
	float* getDetailNoiseWeightPtr() { return &detailNoiseWeight; }
	glm::vec3 getDetailWeights() { return detailWeights; }
	void setDetailWeights(glm::vec3 detailWeights) { this->detailWeights = detailWeights; }
	glm::vec3* getDetailWeightsPtr() { return &detailWeights; }
	glm::vec4 getShapeNoiseWeights() { return shapeNoiseWeights; }
	void setShapeNoiseWeights(glm::vec4 shapeNoiseWeights) { this->shapeNoiseWeights = shapeNoiseWeights; }
	glm::vec4* getShapeNoiseWeightsPtr() { return &shapeNoiseWeights; }
	glm::vec4 getPhaseParams() { return phaseParams; }
	void setPhaseParams(glm::vec4 phaseParams) { this->phaseParams = phaseParams; }
	glm::vec4* getPhaseParamsPtr() { return &phaseParams; }

	glm::vec3 getShapeOffset() { return shapeOffset; }
	void setShapeOffset(glm::vec3 shapeOffset) { this->shapeOffset = shapeOffset; }
	glm::vec3* getShapeOffsetPtr() { return &shapeOffset; }
	glm::vec3 getDetailOffset() { return detailOffset; }
	void setDetailOffset(glm::vec3 detailOffset) { this->detailOffset = detailOffset; }
	glm::vec3* getDetailOffsetPtr() { return &detailOffset; }

	float getCloudSpeed() { return cloudSpeed; }
	void setCloudSpeed(float cloudSpeed) { this->cloudSpeed = cloudSpeed; }
	float* getCloudSpeedPtr() { return &cloudSpeed; }

	// raymarch settings
	int getLightmarchStepCount() { return lightmarchStepCount; }
	int* getLightmarchStepCountPtr() { return &lightmarchStepCount; }
	int getRaymarchStepCount() { return raymarchStepCount; }
	int* getRaymarchStepCountPtr() { return &raymarchStepCount; }
	float getRaymarchMinStepSize() { return raymarchMinStepSize; }
	float* getRaymarchMinStepSizePtr() { return &raymarchMinStepSize; }
	float getRayOffsetStrength() { return rayOffsetStrength; }
	float* getRayOffsetStrengthPtr() { return &rayOffsetStrength; }

	// light settings
	float getLightAbsorptionThroughCloud() { return lightAbsorptionThroughCloud; }
	float* getLightAbsorptionThroughCloudPtr() { return &lightAbsorptionThroughCloud; }
	float getLightAbsorptionTowardSun() { return lightAbsorptionTowardSun; }
	float* getLightAbsorptionTowardSunPtr() { return &lightAbsorptionTowardSun; }
	float getDarknessThreshold() { return darknessThreshold; }
	float* getDarknessThresholdPtr() { return &darknessThreshold; }
	glm::vec4 getLightColor() { return lightColor; }
	void setLightColor(glm::vec4 lightColor) { this->lightColor = lightColor; }
	glm::vec4* getLightColorPtr() { return &lightColor; }
	glm::vec3 getSunDir() { return sunDir; }
	void setSunDir(glm::vec3 sunDir) { this->sunDir = sunDir; }
	glm::vec3* getSunDirPtr() { return &sunDir; }
	
private:

	// cubemap
	void loadCubemapTexturesInternal(ofImage posX, ofImage negX,
									 ofImage posY, ofImage negY,
									 ofImage posZ, ofImage negZ,
									 int size);

	bool cubemapLoaded;
	int nbMipLevels;
	unsigned int cubemapTextureID;
	unsigned int cubemapVAO, cubemapVBO;
	ofShader cubemapShader;

	// volumetric clouds
	bool volumetricCloudActive;
	unsigned int quadVAO, quadVBO;
	ofShader volumetricCloudShader;
	Texture3D cloudNoiseTexture;
	ofImage blueNoiseTexture;

	// cloud settings
	float cloudLayerStartHeight;
	float cloudLayerEndHeight;
	float worldDiameter;
	glm::vec3 worldOrigin;

	float densityMultiplier;
	float densityOffset;
	float noiseScale;
	float detailNoiseScale;
	float detailNoiseWeight;
	glm::vec3 detailWeights;
	glm::vec4 shapeNoiseWeights;
	glm::vec4 phaseParams;

	glm::vec3 shapeOffset;
	glm::vec3 detailOffset;

	float cloudSpeed;

	// raymarch settings
	int lightmarchStepCount;
	int raymarchStepCount;
	float raymarchMinStepSize;
	float rayOffsetStrength;

	// light settings
	float lightAbsorptionThroughCloud;
	float lightAbsorptionTowardSun;
	float darknessThreshold;
	glm::vec4 lightColor;
	glm::vec3 sunDir;

};