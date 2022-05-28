#include "Pipeline.h"

const Matrix4f* Pipeline::getTransformation() {
    Matrix4f ScaleTrans, RotateTrans, TranslationTrans, CameraTranslationTrans, CameraRotateTrans, PersProjTrans;

    ScaleTrans.InitScaleTransform(scale.x, scale.y, scale.z);
    RotateTrans.InitRotateTransform(rotateInfo.x, rotateInfo.y, rotateInfo.z);
    TranslationTrans.InitTranslationTransform(worldPos.x, worldPos.y, worldPos.z);
    CameraTranslationTrans.InitTranslationTransform(-camera.Pos.x, -camera.Pos.y, -camera.Pos.z);
    CameraRotateTrans.InitCameraTransform(camera.Target, camera.Up);
    PersProjTrans.InitPersProjTransform(persProj.FOV, persProj.Width, persProj.Height, persProj.zNear, persProj.zFar);

    transformation = PersProjTrans * CameraRotateTrans * CameraTranslationTrans * TranslationTrans * RotateTrans * ScaleTrans;
    return &transformation;
}