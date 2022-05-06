#include "sceneEnvironnement.h"

void SceneEnvironnement::setup()
{
	setupShaders();
	setupVolumeTextures();

	if (quadVAO == 0)
	{
		float quadVertices[] = 
		{
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		// screen quad VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	}

	blueNoiseTexture.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	blueNoiseTexture.load("resources/clouds/blueNoise/BlueNoise3.png");
}

void SceneEnvironnement::setupShaders()
{
	volumetricCloudShader.unload();
	volumetricCloudShader.load("resources/shader/volumetricClouds/volumetricClouds.vert", "resources/shader/volumetricClouds/volumetricClouds.frag") ?
		ofLog() << "volumetric cloud shader properly loaded" :
		ofLog() << "volumetric cloud not properly loaded";
}

void SceneEnvironnement::setupVolumeTextures()
{
	vector<string> textures;

	for(int i = 0; i < 128; i++)
	{
		string index = to_string(i);
		int zeroCount = 4 - index.length();
		string textureIndex = string(zeroCount, '0');
		textureIndex += index;

		string texturePath = "resources/clouds/3dCloudTextures/STRONG_NOISE/ZSlice_";
		texturePath += textureIndex;
		texturePath += ".png";
		textures.push_back(texturePath);
	}


	if(!cloudNoiseTexture.loadTextures(textures, 128, 128, 128, GL_RGB))
	{
		ofLog() << "cloud volume not loaded properly";
	}
}

bool SceneEnvironnement::loadCubemapTextures(vector<string> texturesPath)
{	
	ofImage cubemapImages[6];	
	bool loaded0 = cubemapImages[0].load(texturesPath[0]);
	bool loaded1 = cubemapImages[1].load(texturesPath[1]);
	bool loaded2 = cubemapImages[2].load(texturesPath[2]);
	bool loaded3 = cubemapImages[3].load(texturesPath[3]);
	bool loaded4 = cubemapImages[4].load(texturesPath[4]);
	bool loaded5 = cubemapImages[5].load(texturesPath[5]);
	
	if( !loaded0 && !loaded1 && !loaded2 && !loaded3 && !loaded4 && !loaded5 ) return false;

	const int size = cubemapImages[0].getWidth();
	nbMipLevels = log2(size) - 1;
		
	//create a texture object
	glGenTextures(1, &cubemapTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, nbMipLevels);

	loadCubemapTexturesInternal(cubemapImages[0], cubemapImages[1], cubemapImages[2], cubemapImages[3], cubemapImages[4], cubemapImages[5], size);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	return true;
}

// Inspired from https://github.com/kalwalt/ofxSkyBox and https://learnopengl.com/Advanced-OpenGL/Cubemaps
void SceneEnvironnement::loadCubemapTexturesInternal(ofImage posX, ofImage negX,
													 ofImage posY, ofImage negY,
													 ofImage posZ, ofImage negZ,
													 int size)
{
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, posX.getPixels().getData()); // positive x
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, posY.getPixels().getData()); // positive y
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, posZ.getPixels().getData()); // positive z	
	
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, negX.getPixels().getData()); // negative x
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, negY.getPixels().getData()); // negative y
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, negZ.getPixels().getData()); // negative z	

	// cubemap VAO
	glGenVertexArrays(1, &cubemapVAO);
	glGenBuffers(1, &cubemapVBO);
	glBindVertexArray(cubemapVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubemapVBO);

	float cubemapVertices[108] = 
	{
		// positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubemapVertices), &cubemapVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	cubemapShader.load("resources/shader/cubemaps/cubemap.vert", "resources/shader/cubemaps/cubemap.frag") ?
	ofLog() << "Cubemap shader properly loaded" :
	ofLog() << "Cubemap shader not properly loaded";

	cubemapLoaded = true;
}