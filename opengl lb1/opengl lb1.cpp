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
#include "shader_codes.h"
#include "Technique.h"
#include "Lighting.h"
#include "Callbacks.h"
#include "GlutBackend.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 720

class Main : ICallbacks {
    GLuint VerticlesBuffer;
    GLuint gWorldLocation;
    GLuint IndexBuffer;
    GLuint Sampler;

    Camera* camera = NULL;
    Texture* texture = NULL;
    LightingTechnique* effect = NULL;
    DirectionLight directionalLight;

    float offset;
    bool pause = false;

    // Создание буфера вершин 
    void CreateVertexBuffer() {
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
    // Создание индексного буфера
    void CreateIndexBuffer() {
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

public:
    Main() {
        camera = NULL;
        texture = NULL;
        effect = NULL;
        offset = 0.0f;
        directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
        directionalLight.AmbientIntensity = 0.5f;
    }
    ~Main() {
        delete camera;
        delete texture;
        delete effect;
    }

    bool Init() {
        camera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

        CreateVertexBuffer();
        CreateIndexBuffer();

        effect = new LightingTechnique();
        if (!effect->Init())
            return false;
       
        effect->Enable();
        effect->SetTextureUnit(0);

        texture = new Texture(GL_TEXTURE_2D, "img\\pepe.jpg");
        if (!texture->Load()) 
            return false;
        
        return true;
    }
    void Run() {
        GLUTBackendRun(this);
    }

    virtual void RenderSceneCB() {
        // Обновление камеры
        camera->OnRender();
        // Очистка буфера каждого кадра
        glClear(GL_COLOR_BUFFER_BIT);

        // Смещение
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

        // Отключение атрибутов вершины
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Меняет фоновый буфер и буфер рендера местами
        glutSwapBuffers();
    }
    virtual void IdleCB() {
        RenderSceneCB();
    }

    // Движение камерой клавиатурой
    virtual void SpecialKeyboardCB(int Key, int x, int y) {
        camera->OnKeyboard(Key);
    }
    // Горячие клавиши
    virtual void KeyboardCB(unsigned char Key, int x, int y) {
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
    virtual void PassiveMouseCB(int x, int y) {
        camera->OnMouse(x, y);
    }
};

int main(int argc, char** argv) {
    GLUTBackendInit(argc, argv);

    if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, ".NET WINDOWS FORMS"))
        return 1;

    Main* simulation = new Main();

    if (!simulation->Init()) {
        return 1;
    }

    simulation->Run();

    delete simulation;
    return 0;
} 