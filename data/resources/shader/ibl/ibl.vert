#version 460

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

uniform mat4 u_viewTranspose;

in vec4 position;
in vec4 color;
in vec3 normal;
in vec2 texcoord;

out vec2 texCoord;
out vec3 norm;
out vec4 pos;
out mat3 tangentView;

void main() 
{
    mat4 normalMatrix = transpose(inverse(modelViewMatrix));

    norm = normalize(vec3(normalMatrix * vec4(normal, 1.0)));
    vec3 c1 = normalize(cross(normal, vec3(0.0, 0.0, 1.0)));
    vec3 c2 = normalize(cross(normal, vec3(0.0, 1.0, 0.0)));
    vec3 tangent = vec3(0.0);

    // Source for tangent calculations : Yasuhiro Hoshino - http://yasuhirohoshino.com/archives/works/ofxibl
    if(length(c1) > length(c2))
    {
        tangent = c1;
    }
    else
    {
        tangent = c2;
    }

    vec3 binormal = normalize(cross(norm, tangent));
    tangentView = mat3(tangent, binormal, norm);
    
    pos = modelViewMatrix * vec4(position.xyz, 1.0);
    texCoord = texcoord;
    
    gl_Position = modelViewProjectionMatrix * position;
}