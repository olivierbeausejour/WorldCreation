#version 460

/*
Tous les calculs/algorithmes mathématiques proviennent des notes de cours
de la session d'hiver 2022 écrites par Philippe Voyer.
*/

in vec2 textureCoords;

// Définis par défaut par OpenFrameworks
uniform sampler2D src_tex_unit0;

uniform float exposureFactor;
uniform float gammaFactor;
uniform int toneMappingTechnique;

out vec4 fragmentColor;

// Tiré des notes de cours, page 157.
vec3 applyExposureFactor(vec3 initialColor)
{
	return vec3(1.0) - exp(-initialColor * exposureFactor);
}

// Tiré des notes de cours, page 158.
vec3 reinhardMapping(vec3 initialColor)
{
	return initialColor / (initialColor + vec3(1.0));
}

// Tiré des notes de cours, page 159.
vec3 acesFilmicMapping(vec3 initialColor)
{
	float aFactor = 2.51;
	float bFactor = 0.03;
	float cFactor = 2.43;
	float dFactor = 0.59;
	float eFactor = 0.14;
	return clamp((initialColor * (aFactor * initialColor + bFactor)) / (initialColor * (cFactor * initialColor + dFactor) + eFactor), 0.0, 1.0);
}

// Tiré des notes de cours, page 161.
vec3 gammaToLinear(vec3 gammaColor, float gammaFactor)
{
	return pow(gammaColor, vec3(gammaFactor));
}

// Tiré des notes de cours, page 162.
vec3 linearToGamma(vec3 linearColor, float gammaFactor)
{
	return pow(linearColor, vec3(1.0 / gammaFactor));
}

void main()
{
	vec3 initialColor = texture(src_tex_unit0, textureCoords).rgb;

	vec3 linearColor = gammaToLinear(initialColor, gammaFactor);
	vec3 exposureColor = applyExposureFactor(linearColor);

	vec3 toneMappedColor;
	if (toneMappingTechnique == 0)
		toneMappedColor = reinhardMapping(exposureColor);
	else
		toneMappedColor = acesFilmicMapping(exposureColor);

	fragmentColor = vec4(linearToGamma(toneMappedColor, gammaFactor), 1.0);
}