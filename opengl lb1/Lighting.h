#ifndef LIGHTING_H
#define	LIGHTING_H

#include "Technique.h"
#include "math_3d.h"

struct DirectionLight {
    Vector3f Color;
    float AmbientIntensity;
};

class LightingTechnique : public Technique {
    GLuint WVPLocation;
    GLuint samplerLocation;
    GLuint dirLightColorLocation;
    GLuint dirLightAmbientIntensityLocation;

public:
    LightingTechnique();
    virtual bool Init();

    void SetWVP(const Matrix4f* WVP);
    void SetTextureUnit(unsigned int TextureUnit);
    void SetDirectionalLight(const DirectionLight& Light);
};


#endif
