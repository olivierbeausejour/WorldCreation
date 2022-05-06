// From https://learnopengl.com/Advanced-OpenGL/Cubemaps

#version 460 core
out vec4 outColor;

in vec3 texCoords;

uniform samplerCube cubemap;

void main()
{    
    outColor = texture(cubemap, texCoords);
}