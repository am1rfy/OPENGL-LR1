#include <iostream>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/vec3.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "glm/mat4x4.hpp"

#include "Pipeline.h"
#include "Camera.h"
#include "math_3d.h"
#include "Texture.h"
#include "Strings.h"
#include "Technique.h"
#include "Lighting.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 720

GLuint VerticlesBuffer;
GLuint gWorldLocation;
GLuint IndexBuffer;
GLuint Sampler;

Camera* camera = NULL;
Texture* texture = NULL;
LightingTechnique* effect = NULL;
DirectionLight directionalLight;

bool pause = false;

static void RenderSceneCB() {
    // Обновление камеры
    camera->OnRender();
    // Очистка буфера каждого кадра
    glClear(GL_COLOR_BUFFER_BIT);

    // Смещение
    static float offset = 0.0f;
    if (!pause) offset += 0.001f;

    //Преобразования
    Pipeline p;
    p.Rotate(0, offset * 30.0f, 0);
    p.WorldPos(0, (abs(sinf(offset) * sinf(offset)) - 0.5f) * 0.15f, 0);
    p.Scale(abs(sinf(offset * 0.1f)), abs(sinf(offset * 0.1f)), abs(sinf(offset * 0.1f)));
    // Перспектива
    p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);
    // Камера
    p.SetCamera(camera->GetPos(), camera->GetTarget(), camera->GetUp());

    effect->SetWVP(p.getTransformation());
    effect->SetDirectionalLight(directionalLight);

    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)p.getTransformation());

    // Использовать атрибуты вершин
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Привязка буфера для отрисовки
    glBindBuffer(GL_ARRAY_BUFFER, VerticlesBuffer);

    // Установка атрибутов вершин
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);

    // Привязка индексного буфера для отрисовки
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer); 
    // Установка текстур
    texture->Bind(GL_TEXTURE0);
    // Отрисовка вершин куба
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // Отрисовка вершин кристалла
    // glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, 0);

    // Отключение атрибутов вершины
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // Меняет фоновый буфер и буфер рендера местами
    glutSwapBuffers();
}

// Движение камерой клавиатурой
static void SpecialKeyboardCB(int Key, int x, int y) {
    camera->OnKeyboard(Key);
}
// Горячие клавиши
static void KeyboardCB(unsigned char Key, int x, int y) {
    switch (Key) {
    case 'q':
        exit(0);
    case 'p':
        if (pause) pause = false;
        else pause = true;
        break;
    case 'a':
        directionalLight.AmbientIntensity += 0.05f;
        break;
    case 's':
        directionalLight.AmbientIntensity -= 0.05f;
        break;
    }
}
// Движение камерой мышью
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
    // Кристалл
    //Vector3f Vertices[10] = {
    //    Vector3f(0.125f, 0.0f, -0.0625f),
    //    Vector3f(-0.125f, 0.0f, -0.0625f),
    //    Vector3f(-0.125f, 0.0f, 0.0625f),
    //    Vector3f(0.125f, 0.0f, 0.0625f),

    //    Vector3f(0.0625f, 0.0f, -0.125f),
    //    Vector3f(-0.0625f, 0.0f, -0.125f),
    //    Vector3f(-0.0625f, 0.0f, 0.125f),
    //    Vector3f(0.0625f, 0.0f, 0.125f),

    //    Vector3f(0.0f, 0.5f, 0.0f),
    //    Vector3f(0.0f, -0.5f, 0.0f),
    //};

    // Куб
    Vertex Vertices[24] = {
        Vertex(Vector3f(-0.25f, 0.25f, 0.25f), Vector2f(0.0f, 0.0f)),
        Vertex(Vector3f(0.25f, 0.25f, 0.25f), Vector2f(1.0f, 0.0f)),
        Vertex(Vector3f(-0.25f, -0.25f, 0.25f), Vector2f(0.0f, 1.0f)),
        Vertex(Vector3f(0.25f, -0.25f, 0.25f), Vector2f(1.0f, 1.0f)),

        Vertex(Vector3f(0.25f, 0.25f, 0.25f), Vector2f(0.0f, 0.0f)),
        Vertex(Vector3f(0.25f, 0.25f, -0.25f), Vector2f(1.0f, 0.0f)),
        Vertex(Vector3f(0.25f, -0.25f, 0.25f), Vector2f(0.0f, 1.0f)),
        Vertex(Vector3f(0.25f, -0.25f, -0.25f), Vector2f(1.0f, 1.0f)),

        Vertex(Vector3f(0.25f, 0.25f, -0.25f), Vector2f(0.0f, 0.0f)),
        Vertex(Vector3f(-0.25f, 0.25f, -0.25f), Vector2f(1.0f, 0.0f)),
        Vertex(Vector3f(0.25f, -0.25f, -0.25f), Vector2f(0.0f, 1.0f)),
        Vertex(Vector3f(-0.25f, -0.25f, -0.25f), Vector2f(1.0f, 1.0f)),

        Vertex(Vector3f(-0.25f, 0.25f, -0.25f), Vector2f(0.0f, 0.0f)),
        Vertex(Vector3f(-0.25f, 0.25f, 0.25f), Vector2f(1.0f, 0.0f)),
        Vertex(Vector3f(-0.25f, -0.25f, -0.25f), Vector2f(0.0f, 1.0f)),
        Vertex(Vector3f(-0.25f, -0.25f, 0.25f), Vector2f(1.0f, 1.0f)),

        Vertex(Vector3f(-0.25f, 0.25f, -0.25f), Vector2f(0.0f, 0.0f)),
        Vertex(Vector3f(0.25f, 0.25f, -0.25f), Vector2f(1.0f, 0.0f)),
        Vertex(Vector3f(-0.25f, 0.25f, 0.25f), Vector2f(0.0f, 1.0f)),
        Vertex(Vector3f(0.25f, 0.25f, 0.25f),Vector2f(1.0f, 1.0f)),

        Vertex(Vector3f(-0.25f, -0.25f, 0.25f), Vector2f(0.0f, 0.0f)),
        Vertex(Vector3f(0.25f, -0.25f, 0.25f), Vector2f(1.0f, 0.0f)),
        Vertex(Vector3f(-0.25f, -0.25f, -0.25f), Vector2f(0.0f, 1.0f)),
        Vertex(Vector3f(0.25f, -0.25f, -0.25f), Vector2f(1.0f, 1.0f)),
    };

    glGenBuffers(1, &VerticlesBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, VerticlesBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

static void CreateIndexBuffer() {
    // Кристалл
    //unsigned int Indices[] = {
    //    5, 4, 8,
    //    4, 0, 8,
    //    0, 3, 8,
    //    3, 7, 8,
    //    7, 6, 8,
    //    6, 2, 8,
    //    2, 1, 8,
    //    1, 5, 8,

    //    5, 4, 9,
    //    4, 0, 9,
    //    0, 3, 9,
    //    3, 7, 9,
    //    7, 6, 9,
    //    6, 2, 9,
    //    2, 1, 9,
    //    1, 5, 9,
    //};

    // Куб
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
    gWorldLocation = glGetUniformLocation(ShaderProgram, "gWVP");
    assert(gWorldLocation != 0xFFFFFFFF);
    Sampler = glGetUniformLocation(ShaderProgram, "gSampler");
    assert(Sampler != 0xFFFFFFFF);
}

int main(int argc, char** argv) {
    // Инициализация окна
    {
        glutInit(&argc, argv);

        // Двойная буферизация и буфер цвета
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

        // Параметры окна
        glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
        glutInitWindowPosition(250, 40);
        glutCreateWindow(".NET WINDOWS FORMS");

        // Отрисовка кадра в окне
        InitializeGlutCallbacks();
    }

    // Инициализация камеры
    camera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

    // Инициализация glew
    {
        GLenum res = glewInit();
        if (res != GLEW_OK) {
            fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
            return 1;
        }
    }
     
    /*glClearColor(1.0f, 1.0f, 0.4f, 0.0f);*/
    glClearColor(0.5f, 0.0f, 1.0f, 0.0f);

    // Оптимизации текстур
    {
        glFrontFace(GL_CW);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
    }

    // Создание буфера вершин
    CreateVertexBuffer();
    CreateIndexBuffer();

    // Создание шейдера
    CompileShaders();

    // Установка индексов текстур, для последующего использования внутри шейдеров
    glUniform1i(Sampler, 0);

    // Инициализация освещения
    effect = new LightingTechnique();
    if (!effect->Init()) 
        return 1;

    effect->Enable();
    effect->SetTextureUnit(0);

    directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
    directionalLight.AmbientIntensity = 0.5f;

    // Инициализация шейдера
    texture = new Texture(GL_TEXTURE_2D, "img\\pepe.jpg");
    if (!texture->Load())
        return 1;

    // glut постоянно вызывает отрисовку
    glutMainLoop();

    return 0;
} 