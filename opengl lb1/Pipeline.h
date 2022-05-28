#ifndef PIPELINE_H
#define	PIPELINE_H

#include "math_3d.h"
#include "glm/glm.hpp"
#include <math.h>
#include "glm/mat4x4.hpp"
#include <glm/fwd.hpp>

class Pipeline {

private:
    Matrix4f transformation;
    Vector3f scale;
    Vector3f worldPos;
    Vector3f rotateInfo;

    struct {
        Vector3f Pos;
        Vector3f Target;
        Vector3f Up;
    } camera;

    struct
    {
        float FOV;
        float Width;
        float Height;
        float zNear;
        float zFar;
    } persProj;

public:
    Pipeline() {
        scale = Vector3f(1.0f, 1.0f, 1.0f);
        worldPos = Vector3f(0.0f, 0.0f, 0.0f);
        rotateInfo = Vector3f(0.0f, 0.0f, 0.0f);
    }

    void Scale(float ScaleX, float ScaleY, float ScaleZ) {
        scale.x = ScaleX;
        scale.y = ScaleY;
        scale.z = ScaleZ;
    }

    void WorldPos(float x, float y, float z) {
        worldPos.x = x;
        worldPos.y = y;
        worldPos.z = z;
    }

    void Rotate(float RotateX, float RotateY, float RotateZ) {
        rotateInfo.x = RotateX;
        rotateInfo.y = RotateY;
        rotateInfo.z = RotateZ;
    }

    void SetPerspectiveProj(float FOV, float Width, float Height, float zNear, float zFar) {
        persProj.FOV = FOV;
        persProj.Width = Width;
        persProj.Height = Height;
        persProj.zNear = zNear;
        persProj.zFar = zFar;
    }

    void SetCamera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up) {
        camera.Pos = Pos;
        camera.Target = Target;
        camera.Up = Up;
    }

    const Matrix4f* getTransformation();
};

#endif