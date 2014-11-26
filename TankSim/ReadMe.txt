##########Contributors##########
Vitaliy Krasylovets 500443090

################################
##########Controls##############
    "UP", "DOWN", "LEFT", "RIGHT" controls the tank movement
    "A", "D" controls camera movement and turret movement
    "Q", "E" controls cannon movement
    "F" fires a round from the position of the cannon
    "N" switches between the Sphere view and Tank View

################################
#######Snipits Usued############
    The Bitmap.h is a snipit of code form one of the examples in class

    In ObjMesh.cpp the function "void load_obj (string filename, ObjMesh **mesh)"
is based on the function form the following link "http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ".
Some of the changes from that function is that we didn't our own VECTOR3D.h instead of external library,
also we load texture courdinates while that fuction doesn't.'
################################