#version 460

// Définis par défaut par OpenFrameworks
in vec4 position;
in vec2 texcoord;
uniform mat4 modelViewProjectionMatrix;

out vec2 textureCoords;

void main()
{
	/* Pas le choix de faire cette passe-passe, car le shader de fragments n'accepte pas directement les attributs variables 
	(il accepte juste ceux du shader de sommets). */
	textureCoords = texcoord;

	gl_Position = modelViewProjectionMatrix * position;
}