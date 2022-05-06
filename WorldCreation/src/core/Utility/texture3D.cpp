#include "core/Utility/texture3D.h"
#include "ofTexture.h"


bool Texture3D::loadTextures(vector<string> textures, int width, int height, int depth, GLenum glFormat)
{
	if(textures.size() != depth) 
	{
		ofLog() << "texture count doesnt match 3d texture depth parameter"; 
		return false;
	}

	unsigned int textureID;
	glGenTextures(1, &textureID);

	this->width = width;
	this->height = height;
	this->depth = depth;

	int channels = 0;
	switch(glFormat)
	{
	case GL_RED:
		channels = 1;
		break;
	case GL_RGB:
		channels = 3;
		break;
	case GL_RGBA:
		channels = 4;
		break;
	}

	unsigned char* textureData = new unsigned char[width * height * depth * channels];

	for(int z = 0; z < depth; z++)
	{
		ofImage currentTexture;
		currentTexture.load(textures[z]);

		if(currentTexture.getWidth() != width || currentTexture.getHeight() != height) 
		{
			ofLog() << "texture width doesnt match width parameter"; 
			return false;
		}

		if(currentTexture.getWidth() != height) 
		{
			ofLog() << "texture height doesnt match height parameter"; 
			return false;
		}

		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				int index = ( (x + width * y) + z * width * height) * channels;
				ofColor col = currentTexture.getColor(x, y);

				switch(glFormat)
				{
				case GL_RED:
					textureData[index] = col.r;
					break;
				case GL_RGB:
					textureData[index] = col.r;
					textureData[index + 1] = col.g;
					textureData[index + 2] = col.b;
					break;
				case GL_RGBA:
					textureData[index] = col.r;
					textureData[index + 1] = col.g;
					textureData[index + 2] = col.b;
					textureData[index + 3] = col.a;
					break;
				}
			}
		}
	}

	glBindTexture(GL_TEXTURE_3D, textureID);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexImage3D(GL_TEXTURE_3D, 0, glFormat, width, height, depth, 0, glFormat, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_3D);

	
	id = textureID;
	return true;
}