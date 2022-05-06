#include "worldComponent.h"

WorldComponent::WorldComponent() : DrawableComponent()
{
	worldMesh = world.getMeshPtr();
	generateHeightMap(); 
	generateWorldMesh();

	renderMode = ofPolyRenderMode::OF_MESH_WIREFRAME;
	worldShader.load("resources/shader/world/worldMapping.vert", "resources/shader/world/worldMapping.frag");
}

// Les concepts derrière l'algorithme sont inspirés de certains concepts de génération procédurale
// expliqués sur ce site (en particulier les premiers) : https://www.redblobgames.com/maps/terrain-from-noise/
void WorldComponent::generateHeightMap()
{
	heightMap.clear();
	heightMap.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_GRAYSCALE);

	for (size_t y = 0; y < heightMap.getHeight(); y++)
	{
		for (size_t x = 0; x < heightMap.getWidth(); x++) {
			float noiseValue = 0;
			float currentAmplitude = startAmplitude;
			float currentFrequency = startFrequency;
			float totalAmplitude = 0;

			for (int i = 1; i <= octaves; i++)
			{
				noiseValue += currentAmplitude * (ofNoise(currentFrequency * (x/MAP_SCALE), currentFrequency * (y/MAP_SCALE)));

				totalAmplitude += currentAmplitude;
				currentAmplitude *= amplitudeFactor;
				currentFrequency *= frequencyFactor;
			}

			noiseValue /= totalAmplitude; // Normalisation entre 0 et 1

			heightMap.setColor(x, y, 255 * noiseValue);
		}
	}

	heightMap.update();
}

void WorldComponent::generateWorldMesh()
{
	worldMesh->setMode(OF_PRIMITIVE_TRIANGLES);
	worldMesh->enableIndices();

	for (size_t z = 0; z < heightMap.getHeight(); z++) 
	{
		for (size_t x = 0; x < heightMap.getWidth(); x++) 
		{
			worldMesh->addVertex(ofVec3f(x, 0, z));
			worldMesh->addTexCoord(ofVec2f(x/heightMap.getWidth(), z/heightMap.getHeight()));
		}
	}

	// Connexion des sommets en triangles
	for (size_t y = 0; y < heightMap.getHeight() - 1; y++) {
		for (size_t x = 0; x < heightMap.getWidth() - 1; x++) {
			worldMesh->addIndex(x + y * heightMap.getWidth());
			worldMesh->addIndex(x + (y + 1) * heightMap.getWidth());
			worldMesh->addIndex(x + 1 + (y + 1) * heightMap.getWidth());

			worldMesh->addIndex(x + y * heightMap.getWidth());
			worldMesh->addIndex(x + 1 + y * heightMap.getWidth());
			worldMesh->addIndex(x + 1 + (y + 1) * heightMap.getWidth());
		}
	}
}

void WorldComponent::draw()
{
	preDraw();

	world.resetTransform();

	world.rotate(transform->getLocalEulerAngles().x, 1, 0, 0);
	world.rotate(transform->getLocalEulerAngles().y, 0, 1, 0);
	world.rotate(transform->getLocalEulerAngles().z, 0, 0, 1);

	world.setPosition(this->transform->getGlobalPosition());
	world.setScale(this->transform->getGlobalScale());

	heightMap.getTextureReference().bind();
	worldShader.begin();

	world.draw(renderMode);

	worldShader.end();
	heightMap.getTextureReference().unbind();

	postDraw();
}

void WorldComponent::setup()
{
	transform->setLocalPosition(glm::vec3(-140, 199.5f, 10));
	transform->setLocalEulerAngles(180, 0, 0);
	setColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
}
