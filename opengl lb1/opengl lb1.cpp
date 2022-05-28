#include <iostream>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/vec3.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "glm/mat4x4.hpp"

#include "Pipeline.h"
#include "Camera.h"
#include "math_3d.h"
#include "Strings.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 720

GLuint VerticlesBuffer;
GLuint gWorldLocation;
GLuint IndexBuffer;

Camera* camera = NULL;

static void RenderSceneCB() {
    camera->OnRender();
    // Очистка буфера каждого кадра
    glClear(GL_COLOR_BUFFER_BIT);

    // Смещение
    static float offset = 0.0f;
    offset += 0.001f;
    // std::cout << offset << std::endl;

    //Преобразования
    Pipeline p;
    p.Scale(sinf(offset * 0.3f), sinf(offset * 0.3f), sinf(offset * 0.3f));
    // p.WorldPos(0, sinf(offset * 0.1f), 0);
    p.Rotate(0, offset * 30.0f, 0);

    p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);
    p.SetCamera(camera->GetPos(), camera->GetTarget(), camera->GetUp());

    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)p.getTransformation());

    // Использовать атрибуты вершин 
    glEnableVertexAttribArray(0);

    // Привязка буфера для отрисовки
    glBindBuffer(GL_ARRAY_BUFFER, VerticlesBuffer);
    // Привязка индексного буфера для отрисовки
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer); 

    // Установка атрибутов вершин
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Отрисовка
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    // Отключение атрибутов вершины
    glDisableVertexAttribArray(0);

    // Меняет фоновый буфер и буфер рендера местами
    glutSwapBuffers();
}

static void SpecialKeyboardCB(int Key, int x, int y) {
    camera->OnKeyboard(Key);
}

static void KeyboardCB(unsigned char Key, int x, int y) {
    switch (Key) {
    case 'q':
        exit(0);
    }
}

static void PassiveMouseCB(int x, int y) {
    camera->OnMouse(x, y);
}

// Инициализация функций-коллбэков для glut
static void InitializeGlutCallbacks() {
    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(RenderSceneCB);
    glutSpecialFunc(SpecialKeyboardCB);
    glutPassiveMotionFunc(PassiveMouseCB);
    glutKeyboardFunc(KeyboardCB);
}

// Метод помещения вектора в буфер
static void CreateVertexBuffer() {   
    //glm::vec3 rectangle[4];

    //rectangle[0] = glm::vec3(0.25f, -0.25f, 0.0f);
    //rectangle[1] = glm::vec3(-0.25f, -0.25f, 0.0f);
    //rectangle[2] = glm::vec3(-0.25f, 0.25f, 0.0f);
    //rectangle[3] = glm::vec3(0.25f, 0.25f, 0.0f);

    /*Vector3f Vertices[8];
    Vertices[0] = Vector3f(0.25f, -0.25f, 0.0f);
    Vertices[1] = Vector3f(-0.25f, -0.25f, 0.0f);
    Vertices[2] = Vector3f(-0.25f, 0.25f, 0.0f);
    Vertices[3] = Vector3f(0.25f, 0.25f, 0.0f);

    Vertices[4] = Vector3f(0.25f, -0.25f, 0.25f);
    Vertices[5] = Vector3f(-0.25f, -0.25f, 0.25f);
    Vertices[6] = Vector3f(-0.25f, 0.25f, 0.25f);
    Vertices[7] = Vector3f(0.25f, 0.25f, 0.25f);*/

    Vector3f Vertices[24] = {
        Vector3f(-0.25f, 0.25f, 0.25f),
        Vector3f(0.25f, 0.25f, 0.25f),
        Vector3f(-0.25f, -0.25f, 0.25f),
        Vector3f(0.25f, -0.25f, 0.25f),

        Vector3f(0.25f, 0.25f, 0.25f),
        Vector3f(0.25f, 0.25f, -0.25f),
        Vector3f(0.25f, -0.25f, 0.25f),
        Vector3f(0.25f, -0.25f, -0.25f),
        
        Vector3f(0.25f, 0.25f, -0.25f),
        Vector3f(-0.25f, 0.25f, -0.25f),
        Vector3f(0.25f, -0.25f, -0.25f),
        Vector3f(-0.25f, -0.25f, -0.25f),

        Vector3f(-0.25f, 0.25f, -0.25f),
        Vector3f(-0.25f, 0.25f, 0.25f),
        Vector3f(-0.25f, -0.25f, -0.25f),
        Vector3f(-0.25f, -0.25f, 0.25f),

        Vector3f(-0.25f, 0.25f, -0.25f),
        Vector3f(0.25f, 0.25f, -0.25f),
        Vector3f(-0.25f, 0.25f, 0.25f),
        Vector3f(0.25f, 0.25f, 0.25f),

        Vector3f(-0.25f, -0.25f, 0.25f),
        Vector3f(0.25f, -0.25f, 0.25f),
        Vector3f(-0.25f, -0.25f, -0.25f),
        Vector3f(0.25f, -0.25f, -0.25f),
    };

    glGenBuffers(1, &VerticlesBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, VerticlesBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

static void CreateIndexBuffer() {
    unsigned int Indices[] = {
        1, 3, 0,
        0, 3, 2,

        9, 11, 8,
        8, 11, 10,

        13, 15, 12,
        12, 15, 14,

        5, 7, 4,
        4, 7, 6,

        17, 19, 16,
        16, 19, 18,

        22, 20, 23,
        23, 20, 21 
    };

    glGenBuffers(1, &IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType) {
    // Создание шейдера
    GLuint ShaderObj = glCreateShader(ShaderType);

    // Обработка ошибок
    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }

    // Сохранение кода шейдера
    const GLchar* p[1];
    p[0] = pShaderText;

    // Массив длин кодов шейдеров
    GLint Lengths[1];
    Lengths[0] = strlen(pShaderText);

    // Задание исходников шейдера
    glShaderSource(ShaderObj, 1, p, Lengths);
    // Компилируем шейдер
    glCompileShader(ShaderObj);

    // Обработка ошибок (шейдер не скомпилился)
    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }

    // Скомпилированный обьект шейдера 
    glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders() {
    // Создание программы шейдера
    GLuint ShaderProgram = glCreateProgram();

    // Обработка ошибки создания программы шейдера
    if (ShaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    // Добавляем шейдер для вершин
    AddShader(ShaderProgram, pVS, GL_VERTEX_SHADER);
    AddShader(ShaderProgram, pFS, GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    // Проверка отклика программы
    glLinkProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    // Валидация программы
    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    // glew теперь использует эту программу шейдера
    glUseProgram(ShaderProgram);

    // сохранение в переменную gWorldLocation
    gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
    assert(gWorldLocation != 0xFFFFFFFF);
}

int main(int argc, char** argv)
{
    // Инициализация окна
    glutInit(&argc, argv);

    // Двойная буферизация и буфер цвета
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    // Параметры окна
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(300, 100);
    glutCreateWindow("DOTNET WINDOWS FORMS");

    // Отрисовка кадра в окне
    InitializeGlutCallbacks();

    // Инициализация камеры
    camera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

    // Инициализация glew
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    // Устанавливается черный цвет при очистке буфера
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Создание буфера вершин
    CreateVertexBuffer();
    CreateIndexBuffer();

    // Создание шейдера
    CompileShaders();

    // glut постоянно вызывает отрисовку
    glutMainLoop();

    return 0;
}