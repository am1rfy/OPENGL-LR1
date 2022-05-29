#include <GL/freeglut.h>
#include "camera.h"
#include "math_3d.h"

const static float StepScale = 0.05f;
const static int MARGIN = 10;

Camera::Camera(int WindowWidth, int WindowHeight) {
    windowWidth = WindowWidth;
    windowHeight = WindowHeight;

    pos = Vector3f(0.0f, 0.0f, 0.0f);
    target = Vector3f(0.0f, 0.0f, 1.0f);
    target.Normalize();
    up = Vector3f(0.0f, 1.0f, 0.0f);

    Init();
}


Camera::Camera(int WindowWidth, int WindowHeight, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up) {
    windowWidth = WindowWidth;
    windowHeight = WindowHeight;

    pos = Pos;

    target = Target;
    target.Normalize();

    up = Up;
    up.Normalize();

    Init();
}

void Camera::Init()
{
    Vector3f HTarget(target.x, 0.0, target.z);
    HTarget.Normalize();

    if (HTarget.z >= 0.0f) {
        if (HTarget.x >= 0.0f) AngleH = 360.0f - ToDegree(asin(HTarget.z));
        else AngleH = 180.0f + ToDegree(asin(HTarget.z));
    }
    else {
        if (HTarget.x >= 0.0f) AngleH = ToDegree(asin(-HTarget.z));
        else AngleH = 90.0f + ToDegree(asin(-HTarget.z));
    }

    AngleV = -ToDegree(asin(target.y));

    OnUpperEdge = false;
    OnLowerEdge = false;
    OnLeftEdge = false;
    OnRightEdge = false;

    mousePos.x = windowWidth / 2;
    mousePos.y = windowHeight / 2;

    glutWarpPointer(mousePos.x, mousePos.y);
}


bool Camera::OnKeyboard(int Key) {
    bool Ret = false;

    switch (Key) {

        case GLUT_KEY_UP: {
            pos -= (target * StepScale);
            Ret = true;
        }
        break;

        case GLUT_KEY_DOWN: {
            pos += (target * StepScale);
            Ret = true;
        }
        break;

        case GLUT_KEY_LEFT: {
            Vector3f Left = target.Cross(up);
            Left.Normalize();
            Left *= StepScale;
            pos += Left;
            Ret = true;
        }
        break;

        case GLUT_KEY_RIGHT: {
            Vector3f Right = up.Cross(target);
            Right.Normalize();
            Right *= StepScale;
            pos += Right;
            Ret = true;
        }
        break;
    }
    return Ret;
}

void Camera::OnMouse(int x, int y) {
    if ((x == mousePos.x) && (y == mousePos.y)) return;

    const int DeltaX = x - mousePos.x;
    const int DeltaY = y - mousePos.y;

    mousePos.x = x;
    mousePos.y = y;

    AngleH += (float)DeltaX / 20.0f;
    AngleV += (float)DeltaY / 20.0f;

    Update();
    glutWarpPointer(mousePos.x, mousePos.y);
}

void Camera::OnRender() {
    bool ShouldUpdate = false;
    if (ShouldUpdate) Update();
}

void Camera::Update() {
    const Vector3f Vaxis(0.0f, 1.0f, 0.0f);

    Vector3f View(1.0f, 0.0f, 0.0f);
    View.Rotate(AngleH, Vaxis);
    View.Normalize();

    Vector3f Haxis = Vaxis.Cross(View);
    Haxis.Normalize();
    View.Rotate(AngleV, Haxis);

    target = View;
    target.Normalize();

    up = target.Cross(Haxis);
    up.Normalize();
}