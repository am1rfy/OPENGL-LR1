# OpenGL first meeting
### Main requerement: https://github.com/microsoft/vcpkg
**Install the necessary dependencies listed in the README of this repository**

### What this software do:
> Draws an arbitrary shape in the window using the DrawPolygon method
```c++
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
```
> Creating a vector with vertices and drawing it
```c++
    std::list <glm::vec3> * myShape = new std::list<glm::vec3>;
    myShape->push_back(glm::vec3(-0.5f, -0.5f, 0.0f)); 
    myShape->push_back(glm::vec3(0.5f, -0.5f, 0.0f)); 
    myShape->push_back(glm::vec3(-0.5f, 0.5f, 0.0f)); 
    myShape->push_back(glm::vec3(0.5f, 0.5f, 0.0f)); 

    DrawPolygon(myShape);
```    

> Result

### Summary
- Display processing: 
```c++
glutDisplayFunc(renderScene);
``` 
- Vertex buffer pointer: 
```c++
GLuint VBO;
``` 
- Buffer handling: 
```c++
  - glGenBuffers(1, &VBO);
  - glBindBuffer(GL_ARRAY_BUFFER, VBO);
  - glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
```  
- Window initialization: 
```c++
  - glutInit(&argc, argv); 
  - glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  - glutInitWindowSize(800, 600);
  - glutInitWindowPosition(300, 100);
  - glutCreateWindow(".NET WINDOWS FORMS"); =)
```
