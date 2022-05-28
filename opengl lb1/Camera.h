#ifndef CAMERA_H
#define	CAMERA_H

#include "glm/mat4x4.hpp"
#include "math_3d.h"
#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)
#define _USE_MATH_DEFINES


class Camera {

private:
    void Init();
    void Update();

    Vector3f pos;
    Vector3f target;
    Vector3f up;

    int windowWidth;
    int windowHeight;

    float AngleH;
    float AngleV;

    bool OnUpperEdge;
    bool OnLowerEdge;
    bool OnLeftEdge;
    bool OnRightEdge;

    Vector2i mousePos;

public:
    Camera(int WindowWidth, int WindowHeight);
    Camera(int WindowWidth, int WindowHeight, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);

    bool OnKeyboard(int Key);
    void OnMouse(int x, int y);
    void OnRender();

    const Vector3f& GetPos() const { return pos; }
    const Vector3f& GetTarget() const { return target; }
    const Vector3f& GetUp() const { return up; }
};

#endif