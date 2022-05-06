#version 460

/*
Tous les calculs/algorithmes mathématiques proviennent des notes de cours
de la session d'hiver 2022 écrites par Philippe Voyer.
*/

const int MAX_NUMBER_OF_LIGHTS = 8;

in vec2 textureCoords;
in vec3 impactPosition;  // Dans l'espace du monde
in vec3 impactNormal;  // Dans l'espace du monde
in vec4 vertColor;

// Définis par défaut par OpenFrameworks
uniform sampler2D src_tex_unit0;
uniform float usingTexture;
uniform mat4 modelMatrix;

uniform vec4 u_materialAmbientColor;
uniform vec4 u_materialDiffuseColor;
uniform int u_isModel;

uniform vec3 u_lightPositions[MAX_NUMBER_OF_LIGHTS];  // Dans l'espace du monde
uniform vec3 u_lightDirections[MAX_NUMBER_OF_LIGHTS];  // Dans l'espace du monde
uniform vec4 u_lightAmbientColors[MAX_NUMBER_OF_LIGHTS];
uniform vec4 u_lightDiffuseColors[MAX_NUMBER_OF_LIGHTS];

uniform float u_lightConstantAttenuations[MAX_NUMBER_OF_LIGHTS];
uniform float u_lightLinearAttenuations[MAX_NUMBER_OF_LIGHTS];
uniform float u_lightQuadraticAttenuations[MAX_NUMBER_OF_LIGHTS];
uniform float u_lightAttenuationPowers[MAX_NUMBER_OF_LIGHTS];
uniform int u_lightTypes[MAX_NUMBER_OF_LIGHTS];

uniform int u_numberOfLights;

out vec4 fragmentColor;

void applyAmbientLight(vec4 lightAmbientColor, inout vec3 currentAmbientColor)
{
	currentAmbientColor += lightAmbientColor.rgb;
}

// Notes de cours page 179
void applyDirectionalLight(vec3 lightDirection, vec4 lightAmbientColor, vec4 lightDiffuseColor, inout vec3 currentAmbientColor, inout vec3 currentDiffuseColor)
{
	vec3 direction = normalize(lightDirection);
	vec3 normal = normalize(impactNormal);

	currentAmbientColor += lightAmbientColor.rgb;	
	currentDiffuseColor += lightDiffuseColor.rgb * max(0.0, dot(normal, direction));	
}

// Notes de cours pages 172 et 179
void applyPointLight(vec3 lightPosition, float lightConstantAttenuation, float lightLinearAttenuation, float lightQuadraticAttenuation, 
					 vec4 lightAmbientColor, vec4 lightDiffuseColor, inout vec3 currentAmbientColor, inout vec3 currentDiffuseColor)
{
	vec3 lightDirection = lightPosition - impactPosition;
	vec3 direction = normalize(lightDirection);
	vec3 normal = normalize(impactNormal);
	float distance = length(lightDirection);
	float attenuation = 1.0 / (lightConstantAttenuation + lightLinearAttenuation * distance + lightQuadraticAttenuation * (distance * distance));

	currentAmbientColor += (lightAmbientColor.rgb * attenuation);	
	currentDiffuseColor += (lightDiffuseColor.rgb * max(0.0, dot(normal, direction)) * attenuation);	
}

// Notes de cours pages 173 et 179
void applySpotlightLight(vec3 lightPosition, vec3 lightDirection, float lightConstantAttenuation, float lightLinearAttenuation, float lightQuadraticAttenuation, float atttenuationPower, 
					     vec4 lightAmbientColor, vec4 lightDiffuseColor, inout vec3 currentAmbientColor, inout vec3 currentDiffuseColor)
{
	vec3 rayDirection = lightPosition - impactPosition;
	vec3 incidentDirection = normalize(-rayDirection);
	vec3 spotDirection = normalize(lightDirection);
	vec3 normal = normalize(impactNormal);

	float distance = length(rayDirection);
	float attenuation = (pow(max(0.0, dot(spotDirection, incidentDirection)), atttenuationPower)) / (lightConstantAttenuation + lightLinearAttenuation * distance + lightQuadraticAttenuation * (distance * distance));

	currentAmbientColor += (lightAmbientColor.rgb * attenuation);	
	currentDiffuseColor += (lightDiffuseColor.rgb * max(0.0, dot(normal, normalize(rayDirection))) * attenuation);	
}

vec4 invertColor(vec4 initialColor)
{
	//Invert values from https://godotshaders.com/shader/invert-color/
	return vec4(1.0 - initialColor.r, 1.0 - initialColor.g, 1.0 - initialColor.b, initialColor.a);
}

void main()
{
	vec3 ambientColor = vec3(0);
    vec3 diffuseColor = vec3(0);

	for (int i = 0; i < u_numberOfLights; i++)
	{
		if (u_lightTypes[i] == 0)
			applyAmbientLight(u_lightAmbientColors[i], ambientColor);
		else if (u_lightTypes[i] == 1)
			applyDirectionalLight(u_lightDirections[i], u_lightAmbientColors[i], u_lightDiffuseColors[i], ambientColor, diffuseColor);
		else if (u_lightTypes[i] == 2)
		{
			applyPointLight(u_lightPositions[i], u_lightConstantAttenuations[i], u_lightLinearAttenuations[i], u_lightQuadraticAttenuations[i], 
							u_lightAmbientColors[i], u_lightDiffuseColors[i], ambientColor, diffuseColor);
		}
		else if (u_lightTypes[i] == 3)
		{
			applySpotlightLight(u_lightPositions[i], u_lightDirections[i], u_lightConstantAttenuations[i], u_lightLinearAttenuations[i], u_lightQuadraticAttenuations[i], 
								u_lightAttenuationPowers[i], u_lightAmbientColors[i], u_lightDiffuseColors[i], ambientColor, diffuseColor);
		}
	}

	if (bool(usingTexture))
	{
		vec4 texColor = texture(src_tex_unit0, textureCoords);
		texColor = invertColor(texColor);
		fragmentColor = vec4((vec4(ambientColor, 1.0) * texColor * u_materialAmbientColor + vec4(diffuseColor, 1.0) * texColor * u_materialDiffuseColor).rgb, texColor.a);
	}
	else if (bool(u_isModel))
	{
		fragmentColor = invertColor(vertColor); 
	}
}