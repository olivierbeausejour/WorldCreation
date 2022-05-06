// From https://learnopengl.com/Advanced-OpenGL/Cubemaps

#version 460 core
layout (location = 0) in vec3 a_position;

out vec3 texCoords;

uniform mat4 projectionMatrix; // Automatically set by OF
uniform mat4 view; // Manually set

void main()
{
    texCoords = a_position;
    vec4 position = -projectionMatrix * view * vec4(a_position, 1.0);
    gl_Position = position.xyww;
}  