#include <io.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "GlutBackend.h"
static ICallbacks* callbacks = NULL;

static void SpecialKeyboardCB(int Key, int x, int y) {
    callbacks->SpecialKeyboardCB(Key, x, y);
}

static void KeyboardCB(unsigned char Key, int x, int y) {
    callbacks->KeyboardCB(Key, x, y);
}

static void PassiveMouseCB(int x, int y) {
    callbacks->PassiveMouseCB(x, y);
}

static void RenderSceneCB() {
    callbacks->RenderSceneCB();
}

static void IdleCB() {
    callbacks->IdleCB();
}

static void InitCallbacks() {
    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(IdleCB);
    glutSpecialFunc(SpecialKeyboardCB);
    glutPassiveMotionFunc(PassiveMouseCB);
    glutKeyboardFunc(KeyboardCB);
}

void GLUTBackendInit(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
}

bool GLUTBackendCreateWindow(unsigned int width, unsigned int height, const char* title) {
    glutInitWindowSize(width, height);
    glutInitWindowPosition(250, 40);
    glutCreateWindow(title);
    
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return false;
    }
    return true;
}

void GLUTBackendRun(ICallbacks* pCallbacks) {
    if (!pCallbacks) {
        fprintf(stderr, "%s : callbacks not specified!\n", __FUNCTION__);
        return;
    }

    glClearColor(0.5f, 0.0f, 1.0f, 0.0f);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    callbacks = pCallbacks;
    InitCallbacks();
    glutMainLoop();
}