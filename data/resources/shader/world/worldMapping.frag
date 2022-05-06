#version 460

in float levelHeight;
out vec4 fragmentColor;

void main()
{
	vec4 tileColor = vec4(0);

	if (levelHeight <= 0.5)
		tileColor = vec4(0.0, 0.4, 0.0, 1.0); // Gazon
	else if (levelHeight < 0.65)
		tileColor = vec4(0.5, 0.5, 0.5, 1.0); // Roches
	else
		tileColor = vec4(1.0); // Neige

	fragmentColor = tileColor;
}