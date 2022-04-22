#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <list>
#include <iostream>
#include "Pipeline.h"
#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

// Буфер вершин
GLuint VerticlesBuffer;
GLuint gWorldLocation;

// Код шейдера
static const char* pVS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
                                                                                    \n\
uniform mat4 gWorld;                                                                \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = gWorld * vec4(Position, 1.0);                                     \n\
}";

static const char* pFS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
out vec4 FragColor;                                                                 \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    FragColor = vec4(1.0, 1.0, 0.0, 1.0);                                           \n\
}";

// vec4(1.0, 1.0, 1.0, 1.0)

static void RenderSceneCB()
{
    // Очистка буфера каждого кадра
    glClear(GL_COLOR_BUFFER_BIT);

    // Смещение
    static float offset = 0.0f;
    offset += 0.001f;

    // Единичная матрица
    glm::mat4x4 unit;
    unit[0][0] = 1.0f; unit[0][1] = 0.0f; unit[0][2] = 0.0f; unit[0][3] = 0.0f;
    unit[1][0] = 0.0f; unit[1][1] = 1.0f; unit[1][2] = 0.0f; unit[1][3] = 0.0f;
    unit[2][0] = 0.0f; unit[2][1] = 0.0f; unit[2][2] = 1.0f; unit[2][3] = 0.0f;
    unit[3][0] = 0.0f; unit[3][1] = 0.0f; unit[3][2] = 0.0f; unit[3][3] = 1.0f;

    // Матрица вращения
    glm::mat4x4 rotate;
    rotate[0][0] = sinf(offset); rotate[0][1] = -sinf(offset); rotate[0][2] = 0.0f; rotate[0][3] = 0.0f;
    rotate[1][0] = 0.0f; rotate[1][1] = 1.0f; rotate[1][2] = 0.0f; rotate[1][3] = 0.0f;
    rotate[2][0] = cosf(offset); rotate[2][1] = cosf(offset); rotate[2][2] = 1.0f; rotate[2][3] = 0.0f;
    rotate[3][0] = 0.0f; rotate[3][1] = 0.0f; rotate[3][2] = 0.0f; rotate[3][3] = 1.0f;

    // Матрица движения
    glm::mat4x4 move;
    move[0][0] = 1.0f; move[0][1] = 0.0f; move[0][2] = 0.0f; move[0][3] = sinf(offset);
    move[1][0] = 0.0f; move[1][1] = 1.0f; move[1][2] = 0.0f; move[1][3] = cosf(offset);
    move[2][0] = 0.0f; move[2][1] = 0.0f; move[2][2] = 1.0f; move[2][3] = 0.0f;
    move[3][0] = 0.0f; move[3][1] = 0.0f; move[3][2] = 0.0f; move[3][3] = 1.0f;

    // Матрица изменения размера
    glm::mat4x4 resize;
    resize[0][0] = sinf(offset); resize[0][1] = 0.0f; resize[0][2] = 0.0f; resize[0][3] = 0.0f;
    resize[1][0] = 0.0f; resize[1][1] = sinf(offset); resize[1][2] = 0.0f; resize[1][3] = 0.0f;
    resize[2][0] = 0.0f; resize[2][1] = 0.0f; resize[2][2] = sinf(offset); resize[2][3] = 0.0f;
    resize[3][0] = 0.0f; resize[3][1] = 0.0f; resize[3][2] = 0.0f; resize[3][3] = 1.0f;

    // Итоговая матрица
    glm::mat4x4 result = unit * rotate * move * resize;
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &result[0][0]);

    //Преобразования
    Pipeline p;
    p.Scale(sinf(offset * 0.1f), sinf(offset * 0.1f), sinf(offset * 0.1f));
    p.WorldPos(sinf(offset), 0.0f, 0.0f);
    p.Rotate(sinf(offset) * 90.0f, sinf(offset) * 90.0f, sinf(offset) * 90.0f);
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)p.getTransformation());

    // Использовать атрибуты вершин
    glEnableVertexAttribArray(0);
    // Привязка буфера для отрисовки
    glBindBuffer(GL_ARRAY_BUFFER, VerticlesBuffer);
    // Установка атрибутов вершин
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Отрисовка
    for (int i = 0; i < 10; i++)
        glDrawArrays(GL_POLYGON, 0, i);

    // Отключение атрибутов вершины
    glDisableVertexAttribArray(0);

    // Меняет фоновый буфер и буфер рендера местами
    glutSwapBuffers();
}

// Инициализация функций-коллбэков для glut
static void InitializeGlutCallbacks()
{
    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(RenderSceneCB);
}

// Метод помещения вектора в буфер
static void CreateVerticlesBuffer(std::list<glm::vec3>*Polygon)
{   
    glm::vec3 localShape[4];

    auto item = Polygon->begin();

    for (int ix = 0; ix < Polygon->size(); ix++) {
        localShape[ix] = *item;
        item++;
    }

    // Генерация обьекта и заполнение буфера
    glGenBuffers(1, &VerticlesBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, VerticlesBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(localShape), localShape, GL_STATIC_DRAW);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    // Создание шейдер
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

static void CompileShaders()
{
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
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(300, 100);
    glutCreateWindow("DOTNET WINDOWS FORMS");

    // Отрисовка кадра в окне
    InitializeGlutCallbacks();

    // Инициализация glew
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    // Устанавливается черный цвет при очистке буфера
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Ромб
    std::list <glm::vec3> * firstShape = new std::list<glm::vec3>;
    firstShape->push_back(glm::vec3(0.5f, -0.5f, 0.0f));
    firstShape->push_back(glm::vec3(-0.5f, -0.5f, 0.0f));
    firstShape->push_back(glm::vec3(-0.5f, 0.5f, 0.0f));
    firstShape->push_back(glm::vec3(0.5f, 0.5f, 0.0f));

    // Создание буфера вершин
    CreateVerticlesBuffer(firstShape);

    // Создание шейдера
    CompileShaders();

    // glut постоянно вызывает отрисовку
    glutMainLoop();

    // Освобождение выделенной памяти
    delete firstShape;
    return 0;
}