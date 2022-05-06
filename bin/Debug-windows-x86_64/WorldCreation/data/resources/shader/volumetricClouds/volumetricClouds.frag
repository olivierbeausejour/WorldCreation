// Inspired greatly from // https://github.com/SebLague/Terraforming
// using same method, with some few differences here in there to convert from hlsl to glsl and
// using different lighting equations
// Used resources:
// https://www.guerrilla-games.com/read/the-real-time-volumetric-cloudscapes-of-horizon-zero-dawn
// https://gist.github.com/pixelsnafu/e3904c49cbd8ff52cb53d95ceda3980e
#version 460 core

out vec4 fragColor;

// screen space position
in vec2 texCoords;

// camera data
uniform float u_nearClip;
uniform float u_farClip;
uniform vec2 u_resolution;
uniform vec2 u_tanFOVby2;
uniform float u_time;

// camera view data
uniform vec3 u_cameraWorldPosition;
uniform mat4 u_inverseProjectionViewMatrix;
uniform mat4 u_projectionViewMatrix;
uniform mat4 u_inverseProjectionMatrix;
uniform mat4 u_projectionMatrix;
uniform mat4 u_inverseViewMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_cameraTransformMatrix;

// cloud noise textures
uniform sampler2D u_frameBufferTexture;
uniform sampler2D u_frameBufferDepth;
uniform sampler3D u_cloudNoiseTexture;
uniform sampler2D u_blueNoiseTexture;

// cloud settings
uniform float u_innerVolumeRadius;
uniform float u_outerVolumeRadius;
uniform vec3 u_worldOrigin;

uniform float u_densityMultiplier;
uniform float u_densityOffset;
uniform float u_noiseScale;
uniform float u_detailNoiseScale;
uniform float u_detailNoiseWeight;
uniform vec3 u_detailWeights;
uniform vec4 u_shapeNoiseWeights;
uniform vec4 u_phaseParams;

uniform vec3 u_shapeOffset;
uniform vec3 u_detailOffset;

uniform float u_cloudSpeed;

// raymarch settings
uniform int u_lightmarchStepCount;
uniform int u_raymarchStepCount;
uniform float u_raymarchMinStepSize;
uniform float u_rayOffsetStrength;

// light settings
uniform float u_lightAbsorptionThroughCloud;
uniform float u_lightAbsorptionTowardSun;
uniform float u_darknessThreshold;
uniform vec4 u_lightColor;
uniform vec3 u_sunDir;


// Returns vector (dstToSphere, dstThroughSphere)
// If ray origin is inside sphere, dstToSphere = 0
// If ray misses sphere, dstToSphere = infinity; dstThroughSphere = 0
vec2 raySphere(vec3 sphereCentre, float sphereRadius, vec3 rayOrigin, vec3 rayDir) {
	vec3 offset = rayOrigin - sphereCentre;
	float a = 1; 
	float b = 2 * dot(offset, rayDir);
	float c = dot(offset, offset) - sphereRadius * sphereRadius;
	float d = b * b - 4 * a * c;

	// Number of intersections: 0 when d < 0; 1 when d = 0; 2 when d > 0
	if (d > 0) {
		float s = sqrt(d);
		float dstToSphereNear = max(0, (-b - s) / (2 * a));
		float dstToSphereFar = (-b + s) / (2 * a);

		// Ignore intersections that occur behind the ray
		if (dstToSphereFar >= 0) {
			return vec2(dstToSphereNear, dstToSphereFar - dstToSphereNear);
		}
	}
	// Ray did not intersect sphere
	return vec2(1.0 / 0.0, 0); // 1.0 / 0.0 == float max value or infinity
}

float sampleCloudDensity(vec3 samplePosition)
{
	float baseScale = 0.0001;

	float finalDensity = 0.0;

	vec3 xyz = samplePosition * baseScale * u_noiseScale;
	vec3 shapeSamplePosition = xyz + (u_shapeOffset + vec3(u_time, 0.0, 0.0) * u_cloudSpeed) * 0.001;

	float gMin = 0.1;
	float gMax = 0.7;
	float heightPercent = (length(u_worldOrigin - samplePosition) - u_innerVolumeRadius) / (u_outerVolumeRadius - u_innerVolumeRadius);

	float heightGradient = clamp(heightPercent/gMin, 0.0, 1.0) * clamp((1 - heightPercent) / (1 - gMax), 0.0, 1.0);

	vec4 shapeNoise = textureLod(u_cloudNoiseTexture, shapeSamplePosition, 0);
	vec4 normalizedShapeWeights = u_shapeNoiseWeights / dot(u_shapeNoiseWeights, vec4(1.0));
	float shapeFBM = dot(shapeNoise, normalizedShapeWeights) * heightGradient;
	float baseShapeDensity = shapeFBM + u_densityOffset * 0.1;

	// Don't sample details if no cloud
	if(baseShapeDensity > 0)
	{
		vec3 detailSamplePosition = xyz * u_detailNoiseScale * u_detailOffset;
		vec4 detailNoise = textureLod(u_cloudNoiseTexture, detailSamplePosition, 0);
		vec3 normalizedDetailWeights = u_detailWeights / dot(u_detailWeights, vec3(1.0));
		float detailFBM = dot(detailNoise.xyz, normalizedDetailWeights);

		float oneMinusShape = 1 - shapeFBM;
		float detailErodeWeight = oneMinusShape * oneMinusShape * oneMinusShape;
		float cloudDensity = baseShapeDensity - (1 - detailFBM) * detailErodeWeight * u_detailNoiseWeight;

		finalDensity = cloudDensity * u_densityMultiplier * 0.1;
	}

	return finalDensity;
}

float henyeyGreenstein(float g, float cosAngle)
{
	float g2 = g * g;
	return (1.0 - g2) / (4 * 3.1415 * pow(1 + g2 - 2 * g * cosAngle, 1.5));
}

float phase(float cosAngle, float phaseDistanceModifier)
{
	float blend = 0.5;
	float hgBlend = henyeyGreenstein(u_phaseParams.x, cosAngle) * (1 - blend) + henyeyGreenstein(u_phaseParams.y, cosAngle) * blend;
	return u_phaseParams.z * phaseDistanceModifier + hgBlend * phaseDistanceModifier * u_phaseParams.w;
}

float beer(float density)
{
	return exp(-density);
}

float lightMarchVolume(vec3 rayPos, float cosAngle)
{
	float dstThroughVolumeToSun = raySphere(u_worldOrigin, u_outerVolumeRadius, rayPos, u_sunDir).y;

	float stepSize = dstThroughVolumeToSun / u_lightmarchStepCount;
	float totalDensity = 0.0;

	for(int step = 0; step < u_lightmarchStepCount; step++)
	{
		rayPos += u_sunDir * stepSize;
		float density = sampleCloudDensity(rayPos);
		totalDensity += max(0, density * stepSize / dstThroughVolumeToSun);
	}

	float transmittance = beer(totalDensity * u_lightAbsorptionTowardSun);
	return u_darknessThreshold + transmittance * (1 - u_darknessThreshold);
}

vec4 marchVolume(vec3 rayPos, vec3 rayDir, float distance, float transmittance, vec3 lightEnergy)
{
	float cosAngle = dot(rayDir, u_sunDir);
	float phaseVal = phase(cosAngle, pow(3.0, -length(rayPos - u_cameraWorldPosition) / 8000.0));
	
	float dstLimit = min(5000, distance);
	float dstTravelled = 0;

	float stepSize = (dstLimit) / (u_raymarchStepCount - 1);
	stepSize = max(u_raymarchMinStepSize, stepSize);
	
	while(dstTravelled < dstLimit)
	{
		float density = sampleCloudDensity(rayPos);

		if(density > 0)
		{
			float lightTransmittance = lightMarchVolume(rayPos, cosAngle);
			lightEnergy += density * stepSize * transmittance * lightTransmittance * phaseVal;
			transmittance *= beer(density * stepSize * u_lightAbsorptionThroughCloud);

			if(transmittance < 0.01)
			{
				break;
			}
		}

		dstTravelled += stepSize;
		rayPos += rayDir * stepSize;
	}

	return vec4(lightEnergy, transmittance);
}

vec2 cloudVolumeIntersect(vec3 rayOrigin, vec3 rayDir)
{
	vec2 rayInnerSphereIntersect = raySphere(u_worldOrigin, u_innerVolumeRadius, rayOrigin, rayDir);
	vec2 rayOuterSphereIntersect = raySphere(u_worldOrigin, u_outerVolumeRadius, rayOrigin, rayDir);

	float dstToInnerSphere = rayInnerSphereIntersect.x;
	float dstThroughInnerSphere = rayInnerSphereIntersect.y;

	float dstToOuterSphere = rayOuterSphereIntersect.x;
	float dstThroughOuterSphere = rayOuterSphereIntersect.y;

	float dstToCenter = length(rayOrigin - u_worldOrigin);

	float dstToVolume = 0;
	float dstThroughVolume = 0;

	if(dstToCenter > u_outerVolumeRadius) // position outside cloud volume
	{
		dstToVolume = dstToOuterSphere;
		dstThroughVolume = (dstThroughInnerSphere > 0) ? dstToInnerSphere - dstToOuterSphere : dstThroughOuterSphere;
	}
	else if(dstToCenter > u_innerVolumeRadius) // position inside cloud volume
	{
		dstToVolume = 0;
		dstThroughVolume = (dstThroughInnerSphere > 0) ? dstToInnerSphere : dstThroughOuterSphere;
	}
	else // position under cloud volume
	{
		dstToVolume = dstThroughInnerSphere;
		dstThroughVolume = dstThroughOuterSphere - dstThroughInnerSphere;
	}

	return vec2(dstToVolume, dstThroughVolume);
}

vec3 getRayDir(float depth, mat4 inverseProjectionView, vec2 texCoords, vec3 cameraWorldPosition)
{
	vec4 clipSpacePosition =  vec4(texCoords * 2.0 - 1.0, depth, 1.0);

	vec4 worldPosition = inverseProjectionView * clipSpacePosition;
	worldPosition = vec4((worldPosition.xyz / worldPosition.w ), 1.0);
	return normalize(worldPosition.xyz - u_cameraWorldPosition);
}

float linearizeDepth(float depth)
{
	float z = depth * 2.0 -1.0;
	return (2.0 * u_nearClip * u_farClip) / (u_farClip + u_nearClip - z * (u_farClip - u_nearClip));
}

void main()
{
	vec4 renderColor = texture(u_frameBufferTexture, texCoords);
	float depth = texture(u_frameBufferDepth, texCoords).r;
	float linearDepth = linearizeDepth(texture(u_frameBufferDepth, texCoords).r);

	if(linearDepth > u_farClip - 1.0)
	{
		linearDepth = 1.0 / 0.0;
	}

	vec3 rayOrigin = u_cameraWorldPosition;
	vec3 rayDir = getRayDir(
		depth,
		u_inverseProjectionViewMatrix,
		texCoords,//uv,
		u_cameraWorldPosition);

	vec2 volumeIntersectInfo = cloudVolumeIntersect(rayOrigin, rayDir);

	float dstToVolume = volumeIntersectInfo.x;
	float dstThroughVolume = min(volumeIntersectInfo.y, linearDepth - dstToVolume);

	if(dstThroughVolume > 0 && (u_cameraWorldPosition + (rayDir * dstToVolume)).y < 0.0)
	{
		// sample cloud lighting in the first volume section
		float randomBlueNoiseOffset = texture(u_blueNoiseTexture, texCoords * 2.5).r * u_rayOffsetStrength;
		vec3 raymarchStartPoint = u_cameraWorldPosition + (rayDir * dstToVolume).xyz;

		float transmittance = 1.0;
		vec3 lightEnergy = vec3(0.0);

		vec4 lightInfo = marchVolume(raymarchStartPoint, rayDir, dstThroughVolume - randomBlueNoiseOffset, transmittance, lightEnergy);
		lightEnergy = lightInfo.xyz;
		transmittance = lightInfo.w;

		// sample cloud lighting in the second volume section
		vec3 rayFirstVolumeExitPosition = u_cameraWorldPosition + rayDir * (dstToVolume + dstThroughVolume + 0.1);
		volumeIntersectInfo = cloudVolumeIntersect(rayFirstVolumeExitPosition, rayDir);
		dstToVolume = volumeIntersectInfo.x;
		dstThroughVolume = min(volumeIntersectInfo.y, linearDepth - dstToVolume);
		
		if(dstThroughVolume > 0 && (u_cameraWorldPosition + (rayDir * dstToVolume)).y < 0.0)
		{
			// sample cloud lighting in the first volume section
			raymarchStartPoint = rayFirstVolumeExitPosition + rayDir * (dstToVolume + randomBlueNoiseOffset);
			vec4 lightInfo = marchVolume(raymarchStartPoint, rayDir, dstThroughVolume - randomBlueNoiseOffset, transmittance, lightEnergy);
			lightEnergy = lightInfo.xyz;
			transmittance = lightInfo.w;
		}

		vec3 cloudColor = u_lightColor.xyz * lightEnergy;
		vec3 finalColor = renderColor.xyz * transmittance + cloudColor;
		fragColor = vec4(finalColor, renderColor.w);
	}
	else
	{
		fragColor = renderColor;
	}
}