# TankSim

TankSim is a crappy tank simulator created using OpenGL 1.2+ with GLU and GLUT libraries.

### Version
1.0.2 (now with Phong Shading!!)

### Tech

TankSim uses a number of open source projects to get to where it is now:

* [OpenGL] - THE open graphics library
* [GLU] - OpenGL utility library for higher-level drawing routines
* [GLUT] - OpenGL utility toolkit for system-level I/O with the OS
* [Blender] - for creating those awesome tank meshes

### Installation

Run the Xcode project. Make sure you've got Xcode 5+ installed.

### Controls

| Action | Result |
| ------ | ------ |
| ```"UP", "DOWN", "LEFT", "RIGHT"``` | controls the tank movement |
| ```"A", "D"``` | controls camera movement and turret movement |
| ```"Q", "E"``` | controls cannon movement |
| ```"F"``` | fires a round from the position of the cannon |
| ```"N"``` | switches between Spherical camera view and Tank view |
| ```"P"``` | activate Phong Shading |
| ```"O"``` | deactivate Phong Shading |

### Todo's

Follow our progress on [Trello]!

### License

None right now?

### Contributors

- Vitaliy Krasylovets - 500443090
- Matthew Tesfaldet - 500389527

### Code Used
- ```Bitmap.h``` is a snippet of code form one of the examples in class
- In ```ObjMesh.cpp``` the function 
```c++
void load_obj (string filename, ObjMesh **mesh)
```
is based on the function from the following link http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ.
Some of the changes of that function is that we use our own VECTOR3D.h instead of the external library's, also we load texture coordinates while that fuction doesn't.
- In ```CityBuilder.cpp```, the functions
```c++
void drawString(const char *str, int x, int y, float color[4], void *font)
```
and
```c++
void showInfo()
```
are taken from http://www.songho.ca/opengl/files/select.zip

[OpenGL]:https://www.opengl.org/
[GLU]:https://www.opengl.org/resources/libraries/
[GLUT]:https://www.opengl.org/resources/libraries/glut/
[Blender]:http://www.blender.org/
[Trello]:https://trello.com/b/FeaxK9sa/tanksim