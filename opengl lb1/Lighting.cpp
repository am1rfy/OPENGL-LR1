#include "Lighting.h"
#include "Strings.h"

LightingTechnique::LightingTechnique() {}

bool LightingTechnique::Init() {
    if (!Technique::Init()) return false;
    if (!AddShader(GL_VERTEX_SHADER, pVS)) return false;
    if (!AddShader(GL_FRAGMENT_SHADER, pFS)) return false;
    if (!Finalize()) return false;
   
    WVPLocation = GetUniformLocation("gWVP");
    samplerLocation = GetUniformLocation("gSampler");
    dirLightColorLocation = GetUniformLocation("gDirectionalLight.Color");
    dirLightAmbientIntensityLocation = GetUniformLocation("gDirectionalLight.AmbientIntensity");

    if (dirLightAmbientIntensityLocation == 0xFFFFFFFF ||
        WVPLocation == 0xFFFFFFFF ||
        samplerLocation == 0xFFFFFFFF ||
        dirLightColorLocation == 0xFFFFFFFF) return false; 

    return true;
}

void LightingTechnique::SetWVP(const Matrix4f* WVP) {
    glUniformMatrix4fv(WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP->m);
}

void LightingTechnique::SetTextureUnit(unsigned int TextureUnit) {
    glUniform1i(samplerLocation, TextureUnit);
}

void LightingTechnique::SetDirectionalLight(const DirectionLight& Light) {
    glUniform3f(dirLightColorLocation, Light.Color.x, Light.Color.y, Light.Color.z);
    glUniform1f(dirLightAmbientIntensityLocation, Light.AmbientIntensity);
}