# OpenGL first meeting
### Main requerement: https://github.com/microsoft/vcpkg
Install the necessary dependencies listed in the README of this repository

### What this software do:
    Draws an arbitrary shape in the window using the DrawPolygon method,
    which is not very flexible yet
    
### Summary
- Display processing: glutDisplayFunc(renderScene);
- Buffer handling: 
  - glGenBuffers(1, &VBO);
  - glBindBuffer(GL_ARRAY_BUFFER, VBO);
  - glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
- Window initialization: 
  - glutInit(&argc, argv); 
  - glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  - glutInitWindowSize(800, 600);
  - glutInitWindowPosition(300, 100);
  - glutCreateWindow(".NET WINDOWS FORMS"); =)
