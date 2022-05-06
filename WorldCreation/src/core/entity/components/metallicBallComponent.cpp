#include "metallicBallComponent.h"

void MetallicBallComponent::draw(const ofCamera * camera, const unsigned int cubemapTextureId, const int nbMipLevels, const bool isCubemapLoaded)
{
    if (!isCubemapLoaded)
    {
        SphereComponent::draw();
        return;
    }

    //Bind cubemap texture
    glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureId);

    iblShader.begin();
    iblShader.setUniform1i("u_envMap", 1);
    iblShader.setUniformTexture("u_normalMap", normalMap.getTexture(), 2);
    iblShader.setUniform1f ("u_normalVal", normalValue);
    iblShader.setUniformMatrix4f("u_viewTranspose", ofMatrix4x4::getTransposedOf(camera->getModelViewMatrix()));
    iblShader.setUniform1f("u_roughness", roughness);
    iblShader.setUniform1f("u_metallic", 1.0f);
    iblShader.setUniform1f("u_cameraFar", camera->getFarClip());
    iblShader.setUniform1i("u_numMips", nbMipLevels);

    ofPushMatrix();
    ofPushStyle();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    SphereComponent::draw();

    glDisable(GL_CULL_FACE);
    ofPopStyle();
    ofPopMatrix();
    
    iblShader.end();
    
    //Unbind cubemap texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glDisable(GL_TEXTURE_CUBE_MAP);
    glActiveTexture(GL_TEXTURE0);
}

int MetallicBallComponent::getMipLevelFromRoughness(const int nbMipLevels)
{
    return min(int(roughness * nbMipLevels), nbMipLevels - 2);
}