#version 460

uniform vec4 globalColor;

uniform samplerCube u_envMap;
uniform sampler2D u_normalMap;
uniform float u_normalVal;
uniform float u_roughness;
uniform float u_metallic;
uniform mat4 u_viewTranspose;
uniform int u_numMips;

in vec4 pos;
in vec3 norm;
in vec2 texCoord;
in mat3 tangentView;

layout(location = 0) out vec4 fragColor;

// Source : Yasuhiro Hoshino - http://yasuhirohoshino.com/archives/works/ofxibl
vec3 PrefilterEnvMap(vec3 rVec)
{
    float levelOfDetail = u_roughness * u_numMips;
    vec4 color = textureLod(u_envMap, rVec, levelOfDetail);
    return color.rgb;
}

// Fonction BRDF approximative par Dimitar Lazarov pour meilleures performances - https://www.unrealengine.com/en-US/blog/physically-based-shading-on-mobile
vec3 EnvBRDFApprox(vec3 specularColor, float nDotV )
{
    vec4 c0 = vec4(-1, -0.0275, -0.572, 0.022);
    vec4 c1 = vec4(1, 0.0425, 1.04, -0.04);
    vec4 r = u_roughness * c0 + c1;
    float a004 = min(r.x * r.x, exp2( -9.28 * nDotV)) * r.x + r.y;
    vec2 AB = vec2(-1.04, 1.04) * a004 + r.zw;
    return specularColor * AB.x + AB.y;
}

// Source : Yasuhiro Hoshino - http://yasuhirohoshino.com/archives/works/ofxibl
vec3 ApproximateSpecularIBL(vec3 specularColor, vec3 nVec, vec3 vVec, vec3 rVec)
{
    float nDotV = dot(nVec, vVec);
    vec3 prefilteredColor = PrefilterEnvMap(rVec);
    return specularColor * prefilteredColor * EnvBRDFApprox(specularColor, nDotV);
}

// Source : Yasuhiro Hoshino - http://yasuhirohoshino.com/archives/works/ofxibl
vec3 Fresnel(vec3 nVec, vec3 vVec, vec3 rVec, float f0)
{
    float base = 1.0 - clamp(dot(nVec, vVec), 0.0, 0.99);
    float exponential = pow(base, 5.0);
    float fresnel = f0 + (1.0 - f0) * exponential;
    vec3 reflectColor = PrefilterEnvMap(rVec);
    return reflectColor * fresnel;
}

void main (void)
{
    vec3 baseColor = globalColor.xyz;
    vec3 viewDir = normalize(-pos.xyz);

	// Source : https://github.com/yasuhirohoshino/ofxIBL/blob/master/src/shaders/IBL.h
    vec3 normalMapVec = texture(u_normalMap, mod(texCoord * vec2(1.0, 1.0), 1.0)).rgb;
    vec3 normal = tangentView * ((mix(vec3(0.5, 0.5, 1.0), normalMapVec, vec3(u_normalVal)) - 0.5) * 2.0).rgb;
    vec3 reflectDir = vec3(u_viewTranspose * vec4(reflect(normalize(pos.xyz), normal), 0.0)) * vec3(1, 1, -1);

    reflectDir.x = -reflectDir.x;
    reflectDir.y = -reflectDir.y;
    
    vec3 diffuseColor = baseColor - baseColor * u_metallic;
    vec3 specularColor	= mix(vec3(0.01), baseColor, u_metallic);
    
    float levelOfDetail = u_roughness * u_numMips;
    vec3 diffuse = textureLod(u_envMap, normal, levelOfDetail).rgb * diffuseColor;
    vec3 specular = ApproximateSpecularIBL(specularColor, normal, viewDir, reflectDir); 
    vec3 fresnel = Fresnel(normal, viewDir, reflectDir, 0.02) * (1.0 - u_metallic);
    
    fragColor = vec4(diffuse + specular + fresnel, globalColor.a);
}