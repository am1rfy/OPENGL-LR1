#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/vec3.hpp"
#include <list>
#include <iostream>

GLuint VBO;

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    for (int i = 0; i < 10; i++)
        glDrawArrays(GL_POLYGON, 0, i);

    glDisableVertexAttribArray(0);

    glutSwapBuffers();
}

static void InitializeGlutCallbacks()
{
    glutDisplayFunc(RenderSceneCB);
}

static void DrawPolygon(std::list<glm::vec3>*Polygon)
{
    glm::vec3 localShape[4];

    auto item = Polygon->begin();

    for (int ix = 0; ix < Polygon->size(); ix++) {
        localShape[ix] = *item;
        item++;
    }

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(localShape), localShape, GL_STATIC_DRAW);
}

static void DrawPoint(glm::vec3 *Point[])
{
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Point), Point, GL_STATIC_DRAW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(300, 100);
    glutCreateWindow("DOTNET WINDOWS FORMS");

    InitializeGlutCallbacks();

    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    std::list <glm::vec3> * myShape = new std::list<glm::vec3>;
    myShape->push_back(glm::vec3(-0.5f, -0.5f, 0.0f)); 
    myShape->push_back(glm::vec3(0.5f, -0.5f, 0.0f)); 
    myShape->push_back(glm::vec3(-0.5f, 0.5f, 0.0f)); 
    myShape->push_back(glm::vec3(0.5f, 0.5f, 0.0f)); 

    DrawPolygon(myShape);

    glutMainLoop();

    delete myShape;
    return 0;
}