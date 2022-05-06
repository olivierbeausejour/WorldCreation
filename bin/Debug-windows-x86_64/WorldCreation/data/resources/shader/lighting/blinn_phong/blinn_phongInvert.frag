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
uniform vec4 u_materialSpecularColor;
uniform int u_isModel;

uniform vec3 u_lightPositions[MAX_NUMBER_OF_LIGHTS];  // Dans l'espace du monde
uniform vec3 u_lightDirections[MAX_NUMBER_OF_LIGHTS];  // Dans l'espace du monde
uniform vec4 u_lightAmbientColors[MAX_NUMBER_OF_LIGHTS];
uniform vec4 u_lightDiffuseColors[MAX_NUMBER_OF_LIGHTS];
uniform vec4 u_lightSpecularColors[MAX_NUMBER_OF_LIGHTS];
uniform float u_lightSpecularFactors[MAX_NUMBER_OF_LIGHTS];

uniform float u_lightConstantAttenuations[MAX_NUMBER_OF_LIGHTS];
uniform float u_lightLinearAttenuations[MAX_NUMBER_OF_LIGHTS];
uniform float u_lightQuadraticAttenuations[MAX_NUMBER_OF_LIGHTS];
uniform float u_lightAttenuationPowers[MAX_NUMBER_OF_LIGHTS];
uniform int u_lightTypes[MAX_NUMBER_OF_LIGHTS];

uniform int u_numberOfLights;
uniform vec3 u_cameraPosition;

out vec4 fragmentColor;

void applyAmbientLight(vec4 lightAmbientColor, inout vec3 currentAmbientColor)
{
	currentAmbientColor += lightAmbientColor.rgb;
}

// Notes de cours pages 169 et 182
void applyDirectionalLight(vec3 lightDirection, float lightSpecularFactor, vec4 lightAmbientColor, vec4 lightDiffuseColor, vec4 lightSpecularColor, 
						   inout vec3 currentAmbientColor, inout vec3 currentDiffuseColor, inout vec3 currentSpecularColor)
{
	vec3 direction = normalize(lightDirection);
	vec3 normal = normalize(impactNormal);

	vec3 l = normalize(u_cameraPosition - impactPosition);	
	vec3 h = normalize(direction + l);

	currentAmbientColor += lightAmbientColor.rgb;	
	currentDiffuseColor += lightDiffuseColor.rgb * max(0.0, dot(normal, direction));	
	currentSpecularColor += lightSpecularColor.rgb * max(0.0, pow(dot(normal, h), lightSpecularFactor));	
}

// Notes de cours pages 169, 172 et 182
void applyPointLight(vec3 lightPosition, float lightSpecularFactor, float lightConstantAttenuation, float lightLinearAttenuation, float lightQuadraticAttenuation, 
					 vec4 lightAmbientColor, vec4 lightDiffuseColor, vec4 lightSpecularColor, inout vec3 currentAmbientColor, inout vec3 currentDiffuseColor, inout vec3 currentSpecularColor)
{
	vec3 lightDirection = lightPosition - impactPosition;
	vec3 direction = normalize(lightDirection);
	vec3 normal = normalize(impactNormal);
	float distance = length(lightDirection);
	float attenuation = 1.0 / (lightConstantAttenuation + lightLinearAttenuation * distance + lightQuadraticAttenuation * (distance * distance));

	vec3 l = normalize(u_cameraPosition - impactPosition);
	vec3 h = normalize(direction + l);

	currentAmbientColor += (lightAmbientColor.rgb * attenuation);	
	currentDiffuseColor += (lightDiffuseColor.rgb * max(0.0, dot(normal, direction)) * attenuation);
	currentSpecularColor += (lightSpecularColor.rgb * max(0.0, pow(dot(normal, h), lightSpecularFactor)) * attenuation);	
}

// Notes de cours pages 169, 173 et 182
void applySpotlightLight(vec3 lightPosition, vec3 lightDirection, float lightSpecularFactor, float lightConstantAttenuation, float lightLinearAttenuation, float lightQuadraticAttenuation, float atttenuationPower, 
					     vec4 lightAmbientColor, vec4 lightDiffuseColor, vec4 lightSpecularColor, inout vec3 currentAmbientColor, inout vec3 currentDiffuseColor, inout vec3 currentSpecularColor)
{
	vec3 rayDirection = lightPosition - impactPosition;
	vec3 incidentDirection = normalize(-rayDirection);
	vec3 spotDirection = normalize(lightDirection);
	vec3 normal = normalize(impactNormal);
	vec3 direction = normalize(rayDirection);

	vec3 l = normalize(u_cameraPosition - impactPosition);	
	vec3 h = normalize(direction + l);

	float distance = length(rayDirection);
	float attenuation = (pow(max(0.0, dot(spotDirection, incidentDirection)), atttenuationPower)) / (lightConstantAttenuation + lightLinearAttenuation * distance + lightQuadraticAttenuation * (distance * distance));

	currentAmbientColor += (lightAmbientColor.rgb * attenuation);	
	currentDiffuseColor += (lightDiffuseColor.rgb * max(0.0, dot(normal, direction)) * attenuation);
	currentSpecularColor += (lightSpecularColor.rgb * max(0.0, pow(dot(normal, h), lightSpecularFactor)) * attenuation);		
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
    vec3 specularColor = vec3(0);

	for (int i = 0; i < u_numberOfLights; i++)
	{
		if (u_lightTypes[i] == 0)
			applyAmbientLight(u_lightAmbientColors[i], ambientColor);
		else if (u_lightTypes[i] == 1)
			applyDirectionalLight(u_lightDirections[i], u_lightSpecularFactors[i], u_lightAmbientColors[i], u_lightDiffuseColors[i], u_lightSpecularColors[i], ambientColor, diffuseColor, specularColor);
		else if (u_lightTypes[i] == 2)
		{
			applyPointLight(u_lightPositions[i], u_lightSpecularFactors[i], u_lightConstantAttenuations[i], u_lightLinearAttenuations[i], u_lightQuadraticAttenuations[i], 
							u_lightAmbientColors[i], u_lightDiffuseColors[i], u_lightSpecularColors[i], ambientColor, diffuseColor, specularColor);
		}
		else if (u_lightTypes[i] == 3)
		{
			applySpotlightLight(u_lightPositions[i], u_lightDirections[i], u_lightSpecularFactors[i], u_lightConstantAttenuations[i], u_lightLinearAttenuations[i], u_lightQuadraticAttenuations[i], 
								u_lightAttenuationPowers[i], u_lightAmbientColors[i], u_lightDiffuseColors[i], u_lightSpecularColors[i], ambientColor, diffuseColor, specularColor);
		}
	}

	if (bool(usingTexture))
	{
		vec4 texColor = texture(src_tex_unit0, textureCoords);
		texColor =  invertColor(texColor);
		fragmentColor = vec4((vec4(ambientColor, 1.0) * texColor * u_materialAmbientColor + vec4(diffuseColor, 1.0) * texColor * u_materialDiffuseColor + vec4(specularColor, 1.0) * texColor * u_materialSpecularColor).rgb, texColor.a);
	}
	else if (bool(u_isModel))
	{
		fragmentColor = invertColor(vertColor); 
	}
}