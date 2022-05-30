#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Pipeline.h"
#include "Camera.h"
#include "Texture.h"
#include "Lighting.h"
#include "GlutBackend.h"
#include "math_3d.h"
#include "Utils.h"

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024

using namespace glm;

struct Vertex
{
    my_Vector3f m_pos;
    vec2 m_tex;
    my_Vector3f m_normal;

    Vertex() {}

    Vertex(my_Vector3f pos, vec2 tex)
    {
        m_pos = pos;
        m_tex = tex;
        m_normal = my_Vector3f(0.0f, 0.0f, 0.0f);
    }
};

class Main : public ICallbacks
{
public:

    Main()
    {
        pGameCamera = NULL;
        pTexture = NULL;
        m_pEffect = NULL;
        Scale = 0.0f;
        directionalLight.Color = my_Vector3f(1.0f, 1.0f, 1.0f);
        directionalLight.AmbientIntensity = 0.5f;
        directionalLight.DiffuseIntensity = 0.75f;
        directionalLight.Direction = my_Vector3f(1.0f, 0.0, 0.0);
    }

    ~Main()
    {
        delete m_pEffect;
        delete pGameCamera;
        delete pTexture;
    }

    bool Init()
    {
        my_Vector3f Pos(0.0f, 0.0f, -3.0f);
        my_Vector3f Target(0.0f, 0.0f, 1.0f);
        my_Vector3f Up(0.0, 1.0f, 0.0f);

        pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

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
                        23, 20, 21 };

        unsigned int Indices2[] = {
                        1, 2, 3,
                        1, 4, 3
        };

        CreateIndexBuffer(Indices, sizeof(Indices));

        CreateVertexBuffer(Indices, ARRAY_SIZE_IN_ELEMENTS(Indices));

        CreateIndexBufferFloor(Indices2, sizeof(Indices2));

        CreateVertexBufferFloor(Indices2, ARRAY_SIZE_IN_ELEMENTS(Indices2));

        m_pEffect = new LightingTechnique();

        if (!m_pEffect->Init())
        {
            printf("Error initializing the lighting technique\n");
            return false;
        }

        m_pEffect->Enable();

        m_pEffect->SetTextureUnit(0);

        pTexture = new Texture(GL_TEXTURE_2D, "img\\pepe.jpg");

        if (!pTexture->Load()) {
            return false;
        }

        return true;
    }

    void Run()
    {
        GLUTBackendRun(this);
    }

    virtual void RenderSceneCB()
    {
        pGameCamera->OnRender();
        glClearColor(0.5f, 0.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (!pause) Scale += 0.001f;

        /*PointLight pl[3];
        pl[0].DiffuseIntensity = 0.5f;
        pl[0].Color = my_Vector3f(1.0f, 0.0f, 0.0f);
        pl[0].Position = my_Vector3f(sinf(Scale) * 10, 1.0f, cosf(Scale) * 10);
        pl[0].Attenuation.Linear = 0.1f;
        pl[1].DiffuseIntensity = 0.5f;
        pl[1].Color = my_Vector3f(0.0f, 1.0f, 0.0f);
        pl[1].Position = my_Vector3f(sinf(Scale + 2.1f) * 10, 1.0f, cosf(Scale + 2.1f) * 10);
        pl[1].Attenuation.Linear = 0.1f;
        pl[2].DiffuseIntensity = 0.5f;
        pl[2].Color = my_Vector3f(0.0f, 0.0f, 1.0f);
        pl[2].Position = my_Vector3f(sinf(Scale + 4.2f) * 10, 1.0f, cosf(Scale + 4.2f) * 10);
        pl[2].Attenuation.Linear = 0.1f;
        m_pEffect->SetPointLights(3, pl);*/

        SpotLight sl[2];
        sl[0].DiffuseIntensity = 15.0f;
        sl[0].Color = my_Vector3f(1.0f, 1.0f, 0.7f);
        sl[0].Position = my_Vector3f(0.0f, 0.0f, 0.0f);
        sl[0].Direction = my_Vector3f(sinf(Scale), 0.0f, cosf(Scale));
        sl[0].Attenuation.Linear = 0.1f;
        sl[0].Cutoff = 20.0f;

        sl[1].DiffuseIntensity = 5.0f;
        sl[1].Color = my_Vector3f(0.0f, 1.0f, 1.0f);
        sl[1].Position = pGameCamera->GetPos();
        sl[1].Direction = pGameCamera->GetTarget();
        sl[1].Attenuation.Linear = 0.1f;
        sl[1].Cutoff = 10.0f;

        m_pEffect->SetSpotLights(2, sl);

        Pipeline p;
        p.Rotate(0, Scale * 30.0f, 0);
        p.WorldPos(0, (abs(sinf(Scale) * sinf(Scale)) - 0.5f) * 0.15f, 0);
        p.Scale(abs(sinf(Scale * 0.1f)), abs(sinf(Scale * 0.1f)), abs(sinf(Scale * 0.1f)));
        // Перспектива
        p.PerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);
        // Камера
        p.SetCamera(pGameCamera->GetPos(), pGameCamera->GetTarget(), pGameCamera->GetUp());

        m_pEffect->SetWVP(p.GetWVPTrans());
        const Matrix4f& WorldTransformation = p.getTransformation();

        m_pEffect->SetWorldMatrix(WorldTransformation);
        m_pEffect->SetDirectionalLight(directionalLight);

        m_pEffect->SetEyeWorldPos(pGameCamera->GetPos());

        m_pEffect->SetMatSpecularIntensity(32.0f);
        m_pEffect->SetMatSpecularPower(1);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        pTexture->Bind(GL_TEXTURE0);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        glutSwapBuffers();
    }

    virtual void IdleCB()
    {
        RenderSceneCB();
    }

    virtual void SpecialKeyboardCB(int Key, int x, int y)
    {
        pGameCamera->OnKeyboard(Key);
    }


    virtual void KeyboardCB(unsigned char Key, int x, int y)
    {
        switch (Key) {
        case 'q':
            glutLeaveMainLoop();
            break;
        case 'p':
            if (pause) pause = false;
            else pause = true;

        case 'a':
            directionalLight.AmbientIntensity += 0.05f;
            break;
        case 's':
            directionalLight.AmbientIntensity -= 0.05f;
            break;

        case 'z':
            directionalLight.DiffuseIntensity += 0.05f;
            break;
        case 'x':
            directionalLight.DiffuseIntensity -= 0.05f;
            break;
        }
    }


    virtual void PassiveMouseCB(int x, int y)
    {
        pGameCamera->OnMouse(x, y);
    }

private:

    void CalcNormals(const unsigned int* pIndices, unsigned int IndexCount,
        Vertex* pVertices, unsigned int VertexCount) {
        for (unsigned int i = 0; i < IndexCount; i += 3) {
            unsigned int Index0 = pIndices[i];
            unsigned int Index1 = pIndices[i + 1];
            unsigned int Index2 = pIndices[i + 2];
            my_Vector3f v1 = pVertices[Index1].m_pos - pVertices[Index0].m_pos;
            my_Vector3f v2 = pVertices[Index2].m_pos - pVertices[Index0].m_pos;
            my_Vector3f Normal = v1.Cross(v2);
            Normal.Normalize();

            pVertices[Index0].m_normal = pVertices[Index0].m_normal + Normal;
            pVertices[Index1].m_normal = pVertices[Index1].m_normal + Normal;
            pVertices[Index2].m_normal = pVertices[Index2].m_normal + Normal;
        }

        for (unsigned int i = 0; i < VertexCount; i++) {
            pVertices[i].m_normal.Normalize();
        }
    }

    void CreateVertexBuffer(const unsigned int* pIndices, unsigned int IndexCount)
    {
        Vertex Vertices[24] = {
                Vertex(my_Vector3f(-0.25f, 0.25f, 0.25f), vec2(0.0f, 0.0f)),
                Vertex(my_Vector3f(0.25f, 0.25f, 0.25f), vec2(1.0f, 0.0f)),
                Vertex(my_Vector3f(-0.25f, -0.25f, 0.25f), vec2(0.0f, 1.0f)),
                Vertex(my_Vector3f(0.25f, -0.25f, 0.25f), vec2(1.0f, 1.0f)),

                Vertex(my_Vector3f(0.25f, 0.25f, 0.25f), vec2(0.0f, 0.0f)),
                Vertex(my_Vector3f(0.25f, 0.25f, -0.25f), vec2(1.0f, 0.0f)),
                Vertex(my_Vector3f(0.25f, -0.25f, 0.25f), vec2(0.0f, 1.0f)),
                Vertex(my_Vector3f(0.25f, -0.25f, -0.25f), vec2(1.0f, 1.0f)),

                Vertex(my_Vector3f(0.25f, 0.25f, -0.25f), vec2(0.0f, 0.0f)),
                Vertex(my_Vector3f(-0.25f, 0.25f, -0.25f), vec2(1.0f, 0.0f)),
                Vertex(my_Vector3f(0.25f, -0.25f, -0.25f), vec2(0.0f, 1.0f)),
                Vertex(my_Vector3f(-0.25f, -0.25f, -0.25f), vec2(1.0f, 1.0f)),

                Vertex(my_Vector3f(-0.25f, 0.25f, -0.25f), vec2(0.0f, 0.0f)),
                Vertex(my_Vector3f(-0.25f, 0.25f, 0.25f), vec2(1.0f, 0.0f)),
                Vertex(my_Vector3f(-0.25f, -0.25f, -0.25f), vec2(0.0f, 1.0f)),
                Vertex(my_Vector3f(-0.25f, -0.25f, 0.25f), vec2(1.0f, 1.0f)),

                Vertex(my_Vector3f(-0.25f, 0.25f, -0.25f), vec2(0.0f, 0.0f)),
                Vertex(my_Vector3f(0.25f, 0.25f, -0.25f), vec2(1.0f, 0.0f)),
                Vertex(my_Vector3f(-0.25f, 0.25f, 0.25f), vec2(0.0f, 1.0f)),
                Vertex(my_Vector3f(0.25f, 0.25f, 0.25f),vec2(1.0f, 1.0f)),

                Vertex(my_Vector3f(-0.25f, -0.25f, 0.25f), vec2(0.0f, 0.0f)),
                Vertex(my_Vector3f(0.25f, -0.25f, 0.25f), vec2(1.0f, 0.0f)),
                Vertex(my_Vector3f(-0.25f, -0.25f, -0.25f), vec2(0.0f, 1.0f)),
                Vertex(my_Vector3f(0.25f, -0.25f, -0.25f), vec2(1.0f, 1.0f)),
        };

        unsigned int VertexCount = ARRAY_SIZE_IN_ELEMENTS(Vertices);
        CalcNormals(pIndices, IndexCount, Vertices, VertexCount);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    }

    void CreateVertexBufferFloor(const unsigned int* pIndices, unsigned int IndexCount)
    {
        Vertex Floor[4] = {
            Vertex(my_Vector3f(-20.0f, -2.0f, -20.0f), vec2(0.0f, 0.0f)),
            Vertex(my_Vector3f(-20.0f, -2.0f, 20.0f), vec2(0.0f, 1.0f)),
            Vertex(my_Vector3f(20.0f, -2.0f, -20.0f), vec2(1.0f, 0.0f)),
            Vertex(my_Vector3f(20.0f, -2.0f, 20.0f), vec2(1.0f, 1.0f))
        };
        unsigned int VertexCount = ARRAY_SIZE_IN_ELEMENTS(Floor);
        CalcNormals(pIndices, IndexCount, Floor, VertexCount);

        glGenBuffers(1, &VBOFloor);
        glBindBuffer(GL_ARRAY_BUFFER, VBOFloor);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Floor), Floor, GL_STATIC_DRAW);
    }
    void CreateIndexBuffer(const unsigned int* pIndices, unsigned int SizeInBytes)
    {
        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, SizeInBytes, pIndices, GL_STATIC_DRAW);
    }

    void CreateIndexBufferFloor(const unsigned int* pIndices, unsigned int SizeInBytes)
    {
        glGenBuffers(1, &IBOFloor);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOFloor);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, SizeInBytes, pIndices, GL_STATIC_DRAW);
    }

    GLuint VBO;
    GLuint VBOFloor;
    GLuint IBO;
    GLuint IBOFloor;
    LightingTechnique* m_pEffect;
    Texture* pTexture;
    Camera* pGameCamera;
    float Scale;
    bool pause;
    DirectionLight directionalLight;
};


int main(int argc, char** argv)
{
    GLUTBackendInit(argc, argv);

    if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 32, false, ".NET WINDOWS FORMS")) {
        return 1;
    }

    Main* pApp = new Main();

    if (!pApp->Init()) {
        return 1;
    }
    pApp->Run();

    delete pApp;

    return 0;
}