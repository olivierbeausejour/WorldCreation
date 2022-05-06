#version 460

// Définis par défaut par OpenFrameworks
in vec4 position;
in vec4 color;
in vec2 texcoord;
uniform mat4 modelViewProjectionMatrix;
uniform sampler2D src_tex_unit0;

out float levelHeight;

void main()
{
	vec4 noiseValue = texture(src_tex_unit0, texcoord);

	levelHeight = clamp(noiseValue.r, 0.5, 1.0);
	float height = max(0.0, levelHeight * 400);

	gl_Position = modelViewProjectionMatrix * vec4(position.x, height, position.z, 1.0);
}