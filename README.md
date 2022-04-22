# OPENGL PART 2

### Main requerement: https://github.com/microsoft/vcpkg
**Install the necessary dependencies listed in the README of this repository**

> Creating a class to merge shader transformations
```c++
  class Pipeline
  {
  private:
      glm::vec3 scale;
      glm::vec3 worldPos;
      glm::vec3 rotateInfo;
      glm::mat4 transformation;

  public:
      Pipeline() {
          scale = glm::vec3(1.0f, 1.0f, 1.0f);
          worldPos = glm::vec3(0.0f, 0.0f, 0.0f);
          rotateInfo = glm::vec3(0.0f, 0.0f, 0.0f);
      }

      void Scale(float ScaleX, float ScaleY, float ScaleZ) {
          scale.x = ScaleX;
          scale.y = ScaleY;
          scale.z = ScaleZ;
      }

      void WorldPos(float x, float y, float z) {
          worldPos.x = x;
          worldPos.y = y;
          worldPos.z = z;
      }

      void Rotate(float RotateX, float RotateY, float RotateZ) {
          rotateInfo.x = RotateX;
          rotateInfo.y = RotateY;
          rotateInfo.z = RotateZ;
      }

      const glm::mat4* getTransformation() {
          glm::mat4 ScaleTrans, RotateTrans, TranslationTrans;
          InitScaleTransform(ScaleTrans);
          InitRotateTransform(RotateTrans);
          InitTranslationTransform(TranslationTrans);
          transformation = TranslationTrans * RotateTrans * ScaleTrans;
          return &transformation;
      }

      void InitScaleTransform(glm::mat4& m) const {
          m[0][0] = scale.x; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
          m[1][0] = 0.0f; m[1][1] = scale.y; m[1][2] = 0.0f; m[1][3] = 0.0f;
          m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = scale.z; m[2][3] = 0.0f;
          m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
      }

      void InitRotateTransform(glm::mat4& m) const {
          glm::mat4 rx, ry, rz;

          const float x = ToRadian(rotateInfo.x);
          const float y = ToRadian(rotateInfo.y);
          const float z = ToRadian(rotateInfo.z);

          rx[0][0] = 1.0f; rx[0][1] = 0.0f; rx[0][2] = 0.0f; rx[0][3] = 0.0f;
          rx[1][0] = 0.0f; rx[1][1] = cosf(x); rx[1][2] = -sinf(x); rx[1][3] = 0.0f;
          rx[2][0] = 0.0f; rx[2][1] = sinf(x); rx[2][2] = cosf(x); rx[2][3] = 0.0f;
          rx[3][0] = 0.0f; rx[3][1] = 0.0f; rx[3][2] = 0.0f; rx[3][3] = 1.0f;

          ry[0][0] = cosf(y); ry[0][1] = 0.0f; ry[0][2] = -sinf(y); ry[0][3] = 0.0f;
          ry[1][0] = 0.0f; ry[1][1] = 1.0f; ry[1][2] = 0.0f; ry[1][3] = 0.0f;
          ry[2][0] = sinf(y); ry[2][1] = 0.0f; ry[2][2] = cosf(y); ry[2][3] = 0.0f;
          ry[3][0] = 0.0f; ry[3][1] = 0.0f; ry[3][2] = 0.0f; ry[3][3] = 1.0f;

          rz[0][0] = cosf(z); rz[0][1] = -sinf(z); rz[0][2] = 0.0f; rz[0][3] = 0.0f;
          rz[1][0] = sinf(z); rz[1][1] = cosf(z); rz[1][2] = 0.0f; rz[1][3] = 0.0f;
          rz[2][0] = 0.0f; rz[2][1] = 0.0f; rz[2][2] = 1.0f; rz[2][3] = 0.0f;
          rz[3][0] = 0.0f; rz[3][1] = 0.0f; rz[3][2] = 0.0f; rz[3][3] = 1.0f;

          m = rz * ry * rx;
      }

      void InitTranslationTransform(glm::mat4& m) const
      {
          m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = worldPos.x;
          m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = worldPos.y;
          m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = worldPos.z;
          m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
      }
  };
```
### Then in RenderSceneCB function:

> Offset
```c++
      static float offset = 0.0f;
      offset += 0.001f;
```

> Unit matrix
```c++
    glm::mat4x4 unit;
    unit[0][0] = 1.0f; unit[0][1] = 0.0f; unit[0][2] = 0.0f; unit[0][3] = 0.0f;
    unit[1][0] = 0.0f; unit[1][1] = 1.0f; unit[1][2] = 0.0f; unit[1][3] = 0.0f;
    unit[2][0] = 0.0f; unit[2][1] = 0.0f; unit[2][2] = 1.0f; unit[2][3] = 0.0f;
    unit[3][0] = 0.0f; unit[3][1] = 0.0f; unit[3][2] = 0.0f; unit[3][3] = 1.0f;
```

> Rotating matrix
```c++
    glm::mat4x4 rotate;
    rotate[0][0] = sinf(offset); rotate[0][1] = -sinf(offset); rotate[0][2] = 0.0f; rotate[0][3] = 0.0f;
    rotate[1][0] = 0.0f; rotate[1][1] = 1.0f; rotate[1][2] = 0.0f; rotate[1][3] = 0.0f;
    rotate[2][0] = cosf(offset); rotate[2][1] = cosf(offset); rotate[2][2] = 1.0f; rotate[2][3] = 0.0f;
    rotate[3][0] = 0.0f; rotate[3][1] = 0.0f; rotate[3][2] = 0.0f; rotate[3][3] = 1.0f;
```

> Movement matrix
```c++
    glm::mat4x4 move;
    move[0][0] = 1.0f; move[0][1] = 0.0f; move[0][2] = 0.0f; move[0][3] = sinf(offset);
    move[1][0] = 0.0f; move[1][1] = 1.0f; move[1][2] = 0.0f; move[1][3] = cosf(offset);
    move[2][0] = 0.0f; move[2][1] = 0.0f; move[2][2] = 1.0f; move[2][3] = 0.0f;
    move[3][0] = 0.0f; move[3][1] = 0.0f; move[3][2] = 0.0f; move[3][3] = 1.0f;
```
> Resizing matrix
```c++
    glm::mat4x4 resize;
    resize[0][0] = sinf(offset); resize[0][1] = 0.0f; resize[0][2] = 0.0f; resize[0][3] = 0.0f;
    resize[1][0] = 0.0f; resize[1][1] = sinf(offset); resize[1][2] = 0.0f; resize[1][3] = 0.0f;
    resize[2][0] = 0.0f; resize[2][1] = 0.0f; resize[2][2] = sinf(offset); resize[2][3] = 0.0f;
    resize[3][0] = 0.0f; resize[3][1] = 0.0f; resize[3][2] = 0.0f; resize[3][3] = 1.0f;
```

> Resulting matrix (mul of all matrices described above)
```c++
    glm::mat4x4 result = unit * rotate * move * resize;
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &result[0][0]);
```

> Transformations of the resultant matrix
```c++
    Pipeline p;
    p.Scale(sinf(offset * 0.1f), sinf(offset * 0.1f), sinf(offset * 0.1f));
    p.WorldPos(sinf(offset), 0.0f, 0.0f);
    p.Rotate(sinf(offset) * 90.0f, sinf(offset) * 90.0f, sinf(offset) * 90.0f);
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)p.getTransformation());
```

### Added AddShader function 

```c++
static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType) {
    // Creating a shader
    GLuint ShaderObj = glCreateShader(ShaderType);

    // Error handling
    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }

    // Saving shader code
    const GLchar* p[1];
    p[0] = pShaderText;

    // Shader Code Length Array
    GLint Lengths[1];
    Lengths[0] = strlen(pShaderText);

    // Setting the shader sources
    glShaderSource(ShaderObj, 1, p, Lengths);
    // Компилируем шейдер
    glCompileShader(ShaderObj);

    // Error handling (shader didn't compile)
    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }

    // Compiled shader object 
    glAttachShader(ShaderProgram, ShaderObj);
}
```

### Added CompileShaders function

```c++
static void CompileShaders()
{
    // Creating a shader program
    GLuint ShaderProgram = glCreateProgram();

    // Handling a shader program creation error
    if (ShaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    // Adding a vertex shader
    AddShader(ShaderProgram, pVS, GL_VERTEX_SHADER);
    AddShader(ShaderProgram, pFS, GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    // Checking program response
    glLinkProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    // Program validation
    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    // GLEW now uses this shader program
    glUseProgram(ShaderProgram);

    // saving to the gWorldLocation variable
    gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
    assert(gWorldLocation != 0xFFFFFFFF);
}
```
