#version 460

// Définis par défaut par OpenFrameworks
in vec4 position;
in vec4 normal;
in vec2 texcoord;
in vec4 color;
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;
uniform int u_invertNormal;


out vec2 textureCoords;
out vec4 vertColor;
out vec3 impactPosition;  // Dans l'espace du monde
out vec3 impactNormal;  // Dans l'espace du monde

void main()
{
	impactPosition = vec3(modelMatrix * position);

	mat4 normalMatrix = transpose(inverse(modelMatrix));
	impactNormal = normalize((normalMatrix * normal).xyz);

	if (bool(u_invertNormal))
		impactNormal = -impactNormal;

	textureCoords = texcoord;
	vertColor = color;

	gl_Position = modelViewProjectionMatrix * position;
}