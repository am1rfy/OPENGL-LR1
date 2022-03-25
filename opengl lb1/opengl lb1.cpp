#include <GL/freeglut.h>
#include <iostream>
#include <stdio.h>

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

static void InitializeGlutCallbacks()
{
    glutDisplayFunc(RenderSceneCB);
}

static void WindowInit(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(150, 100);
    glutCreateWindow("DOTNET WINDOWS FORMS");
    InitializeGlutCallbacks();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glutMainLoop();
}

int main(int argc, char** argv)
{
    WindowInit(argc, argv);
    return 0;
}