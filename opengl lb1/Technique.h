#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#include <GL/glew.h>
#include <list>

class Technique {
    GLuint shaderProg;
    typedef std::list<GLuint> ShaderObjList;
    ShaderObjList shaderObjList;

protected:
    bool AddShader(GLenum ShaderType, const char* pShaderText);
    bool Finalize();
    GLint GetUniformLocation(const char* pUniformName);

public:
    Technique();
    ~Technique();
    virtual bool Init();
    void Enable();
};

#endif 