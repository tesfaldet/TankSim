/*******************************************************************
 City Modelling Program
 ********************************************************************/
//////////
#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <utility>
#include <vector>
#include "VECTOR3D.h"
#include <string>

#include "TerrainGrid.h"
#include "Mesh.h"
#include "Bitmap.h"
#include "ObjMesh.h"
#include "Tank.h"

void initOpenGL();
void display(void);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseMotionHandler(int xMouse, int yMouse);
void keyboard(unsigned char key, int x, int y);
void functionUpKeys(int key, int x, int y);
void functionKeys(int key, int x, int y);
void timer(int value);
VECTOR3D ScreenToWorld(int x, int y);
void updateCameraPos();
float* calculateBoundingBox(BuildingMesh* mesh);
bool checkForCollision(BuildingMesh* mesh1, BuildingMesh* mesh2);
bool checkCollisionWithBuildings();
void limitCameraAngle();
void animationFunction (float delta_time);
void loadTank(Tank **tank);

static int currentButton;
static unsigned char currentKey;
static int currentFuncKey;
const float FPS = 30.0;
int count = 0;

// City Interaction State Variable
enum Action {TRANSLATE, ROTATE, SCALE, EXTRUDE, SELECT, MULTIPLESELECT, DESELECT_ALL, NAVIGATE};
enum Action currentAction = TRANSLATE;

GLfloat light_position0[] = {12.0, 6.0,-12.0,1.0};
GLfloat light_position1[] = {-12.0, 6.0,12.0,1.0};
GLfloat light_diffuse[]   = {1.0, 1.0, 1.0, 1.0};
GLfloat light_specular[]  = {1.0, 1.0, 1.0, 1.0};
GLfloat light_ambient[]   = {0.2, 0.2, 0.2, 1.0};


// Building meshes
Mesh *buildings[25];
int numBuildings = 0;

float buildingFloorHeight = 0.25;	// Building Floor Height for city building

// Street meshes
Mesh *streets[11];
int numStreets = 0;

// Vehicle mesh
Mesh *vehicle;

// City terrain mesh
TerrainGrid *terrainGrid = NULL;
int gridSize = 16;


// Camera Control
VECTOR3D lookFrom;
VECTOR3D lookAt;
VECTOR3D up;

float radius = 12;			// Camera Distance
float lookFromx = 0;		// Camera X Position
float lookFromy = 0;	// Camera Y Position
float lookFromz = radius;		// Camera Z Position

float angleTheta = 0;		// Camera X angle
float anglePhi = 80;		// Camera Y angle

float upx = 0;			// Up Vector
float upy = 1;
float upz = 0;

float lookAtx = 0;		// Camera is looking at
float lookAty = 0;
float lookAtz = 0;

float camerax = 0;		// Camera X Position
float cameray = 0;	// Camera Y Position
float cameraz = radius;		// Camera Z Position


static float zoomFactor = 1.0;

float xbefore;			// Previous X position of tank
float zbefore;			// Previous Y position of tank


GLint glutWindowWidth    = 750;
GLint glutWindowHeight   = 500;
GLint viewportWidth  = glutWindowWidth;
GLint viewportHeight = glutWindowHeight;

// Wolrd Boundaries
GLdouble worldLeftBase  =  -8.0;
GLdouble worldRightBase =  8.0;
GLdouble worldBottomBase=  -8.0;
GLdouble worldTopBase   =  8.0;

// World view boundaries
GLdouble wvLeftBase		=  worldLeftBase,
wvRightBase	=  worldRightBase,
wvBottomBase	=  worldBottomBase,
wvTopBase		=  worldTopBase;

//Building Textures
RGBpixmap bulding_pixelMap[3];
char building_fileName1[] = "textures/building1.bmp";
char building_fileName2[] = "textures/building2.bmp";
char building_fileName3[] = "textures/building3.bmp";
//Road Textures
RGBpixmap road_pixelMap;
char road_fileName[] = "textures/road.bmp";
//Terrain Textures
RGBpixmap terrain_pixelMap;
char terrain_fileName[] = "textures/grass.bmp";

//Tank
std::string tank_fileName("tank1/tank.obj");
std::string cannon_fileName("tank1/cannon.obj");
std::string turret_fileName("tank1/turret.obj");
std::string wheel1_fileName("tank1/wheel1.obj");
std::string wheel2_fileName("tank1/wheel2.obj");
std::string wheel3_fileName("tank1/wheel3.obj");
std::string wheel4_fileName("tank1/wheel4.obj");

int num_of_tanks = 1;
int selected_tank = 0;

Tank *tank[3];

RGBpixmap tank_body_pixelMap;
RGBpixmap tank_cannon_pixelMap;
RGBpixmap tank_turret_pixelMap;
RGBpixmap tank_wheel_pixelMap;

char tank_body_fileName[] = "tank1/body.bmp";
char tank_cannon_fileName[] = "tank1/cannon.bmp";
char tank_turret_fileName[] = "tank1/turret.bmp";
char tank_wheel_fileName[] = "tank1/wheel.bmp";


int main(int argc, char **argv)
{
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(750, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("City Navigator");
  
  initOpenGL();
  
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(mouseMotionHandler);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(functionKeys);
  glutSpecialUpFunc(functionUpKeys);
  glutTimerFunc(1000.0 / FPS, timer, 0);
  glutMainLoop();
  return 0;
}


int width, height;

// Setup openGL */
void initOpenGL()
{
  glViewport(0, 0, (GLsizei) viewportWidth, (GLsizei) viewportHeight);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0*zoomFactor,(float)viewportWidth/(float)viewportHeight,0.2,80.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  // Set up and enable lighting
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
  
  glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glClearColor(0.6, 0.6, 0.6, 0.0);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  //Nice perspective.
  glHint(GL_PERSPECTIVE_CORRECTION_HINT , GL_NICEST);
  
  // Set up Terrain Grid
  VECTOR3D origin = VECTOR3D(-16.0f,0.0f,16.0f);
  terrainGrid = new TerrainGrid(gridSize, 32.0);
  terrainGrid->InitGrid(gridSize, origin, 32.0, 32.0);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  updateCameraPos();
  gluLookAt(lookFromx, lookFromy, lookFromz,lookAtx, lookAty, lookAtz, upx, upy, upz);
  
  
  VECTOR3D scale;
  VECTOR3D trans;
  VECTOR3D angles;
  
  // Create a building
  int numFloors = 10;
  scale.x = scale.z = 0.5;
  scale.y = 1.0;
  trans.x = -3.0;
  trans.z = -1.0;
  trans.y = 0;
  
  buildings[0] = createMesh(scale, trans, buildingFloorHeight, numFloors);
  buildings[1] = createMesh(scale, VECTOR3D(0,0,0), buildingFloorHeight, 20);
  buildings[2] = createMesh(scale, VECTOR3D(0,0,5), buildingFloorHeight, 25);
  buildings[3] = createMesh(scale, VECTOR3D(-2,0,3), buildingFloorHeight, 20);
  buildings[4] = createMesh(scale, VECTOR3D(5,0,2), buildingFloorHeight, 20);
  buildings[5] = createMesh(scale, VECTOR3D(1,0,2), buildingFloorHeight, 30);
  buildings[6] = createMesh(scale, VECTOR3D(1,0,6), buildingFloorHeight, 20);
  buildings[7] = createMesh(scale, VECTOR3D(1.8,0,0), buildingFloorHeight, 10);
  buildings[8] = createMesh(scale, VECTOR3D(3.8,0,5), buildingFloorHeight, 20);
  buildings[9] = createMesh(scale, VECTOR3D(5,0,6), buildingFloorHeight, 13);
  buildings[10] = createMesh(scale, VECTOR3D(1.8,0,-6), buildingFloorHeight, 22);
  buildings[11] = createMesh(scale, VECTOR3D(5,0,-4), buildingFloorHeight, 15);
  buildings[12] = createMesh(scale, VECTOR3D(8,0,-6.0), buildingFloorHeight, 18);
  buildings[13] = createMesh(scale, VECTOR3D(8,0,-3), buildingFloorHeight, 22);
  buildings[14] = createMesh(scale, VECTOR3D(8,0,3), buildingFloorHeight, 40);
  numBuildings = 15;
  
  // Create a street
  scale.z = 0.1;
  scale.x = 0.5;
  scale.y = 1.0;
  trans.x = -4.0;
  trans.y = -0.08;
  trans.z = -8.0;
  
  streets[0] = createMesh(scale, trans, 1.0, 15);
  streets[0]->angles.x = 90.0;
  streets[0]->angles.y = 00.0;
  streets[0]->angles.z = 00.0;
  streets[1] = createMesh(scale, VECTOR3D(-4.5,-0.08,-8.0), 1.0, 15);
  streets[1]->angles.x = 90.0;
  streets[1]->angles.y = 90.0;
  streets[1]->angles.z = 00.0;
  streets[2] = createMesh(scale, VECTOR3D(-4.5,-0.08,-5.0), 1.0, 15);
  streets[2]->angles.x = 90.0;
  streets[2]->angles.y = 90.0;
  streets[2]->angles.z = 00.0;
  streets[3] = createMesh(scale, VECTOR3D(-4.5,-0.08,-2.0), 1.0, 15);
  streets[3]->angles.x = 90.0;
  streets[3]->angles.y = 90.0;
  streets[3]->angles.z = 00.0;
  streets[4] = createMesh(scale, VECTOR3D(-4.5,-0.08,1.0), 1.0, 15);
  streets[4]->angles.x = 90.0;
  streets[4]->angles.y = 90.0;
  streets[4]->angles.z = 00.0;
  streets[5] = createMesh(scale, VECTOR3D(-4.5,-0.08,4.0), 1.0, 15);
  streets[5]->angles.x = 90.0;
  streets[5]->angles.y = 90.0;
  streets[5]->angles.z = 00.0;
  streets[6] = createMesh(scale, VECTOR3D(-4.5,-0.08,7.0), 1.0, 15);
  streets[6]->angles.x = 90.0;
  streets[6]->angles.y = 90.0;
  streets[6]->angles.z = 00.0;
  streets[7] = createMesh(scale, VECTOR3D(10,-0.08,-8.0), 1.0, 15);
  streets[7]->angles.x = 90.0;
  streets[7]->angles.y = 00.0;
  streets[7]->angles.z = 00.0;
  streets[8] = createMesh(scale, VECTOR3D(2.75,-0.08,-8.0), 1.0, 15);
  streets[8]->angles.x = 90.0;
  streets[8]->angles.y = 00.0;
  streets[8]->angles.z = 00.0;
  streets[9] = createMesh(scale, VECTOR3D(6.5,-0.08,-8.0), 1.0, 15);
  streets[9]->angles.x = 90.0;
  streets[9]->angles.y = 00.0;
  streets[9]->angles.z = 00.0;
  streets[10] = createMesh(scale, VECTOR3D(-1.0,-0.08,-8.0), 1.0, 15);
  streets[10]->angles.x = 90.0;
  streets[10]->angles.y = 00.0;
  streets[10]->angles.z = 00.0;
  numStreets = 11;
  
  // Create a vehicle - initially oriented along z axis direction
  scale.x = 0.125;
  scale.z = 0.25;
  scale.y = 1.0;
    trans.x = 7.0;// -4.0;
    trans.z = 7.0;//6.0;
  trans.y = 0;
  
  vehicle = createMesh(scale, trans, 0.125, 2);
  vehicle->angles.x = vehicle->angles.y = vehicle->angles.z = 0.0;
  vehicle->selected = true;
    
  //Texturing
  glEnable(GL_TEXTURE_2D);
    
  //Load building textures
  readBMPFile(&bulding_pixelMap[0],building_fileName1);
  setTexture(&bulding_pixelMap[0],2001);
  
  readBMPFile(&bulding_pixelMap[1],building_fileName2);
  setTexture(&bulding_pixelMap[1],2002);
  
  readBMPFile(&bulding_pixelMap[2],building_fileName3);
  setTexture(&bulding_pixelMap[2],2003);
  
  //Road Textures
  readBMPFile(&road_pixelMap, road_fileName);
  setTexture(&road_pixelMap, 2004);
  
  // Terrain textures
  readBMPFile(&terrain_pixelMap, terrain_fileName);
  setTexture(&terrain_pixelMap, 2005);
  
  //Set Texture for buildings
  for (int i = 0; i < numBuildings; i++) {
      if ((i % 2) == 0) {
        setTextureMapID(buildings[i],2001);
      } else if ((i % 3) == 0) {
        setTextureMapID(buildings[i],2002);
      } else {
        setTextureMapID(buildings[i],2003);
      }

  }
    
  //Set Texture for streets
  for (int i = 0; i < numStreets; i++) {
    setTextureMapID(streets[i], 2004);
  }
    
  //Set Texture for Terrain
  terrainGrid->setTextureID(2005);
    
  readBMPFile(&tank_body_pixelMap, tank_body_fileName);
  setTexture(&tank_body_pixelMap, 2006);
    
  readBMPFile(&tank_cannon_pixelMap, tank_cannon_fileName);
  setTexture(&tank_cannon_pixelMap, 2007);
    
  readBMPFile(&tank_turret_pixelMap, tank_turret_fileName);
  setTexture(&tank_turret_pixelMap, 2008);
    
  readBMPFile(&tank_wheel_pixelMap, tank_wheel_fileName);
  setTexture(&tank_wheel_pixelMap, 2009);
    
  for(int i = 0; i < num_of_tanks; i++){
     // loadTank(&tank[i]);
  }
    
    loadTank(&tank[0]);
    
    tank[0]->moveBy(VECTOR3D(7.0,0.0,7.0));
  //tank[0]->moveBy(VECTOR3D(7.0,0.0,7.0));
 // tank[0]->rotateCannon(40);
  //tank[0]->rotateTurret(-30);
    
//  tank[1]->moveBy(VECTOR3D(8.0,0.0,7.0));
 // tank[2]->moveBy(VECTOR3D(7.0,0.0,9.0));
}


void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    
  glLoadIdentity();
  
  if (currentAction == NAVIGATE)
    ;
  else
    updateCameraPos();
  
  gluLookAt(lookFromx, lookFromy, lookFromz, lookAtx, lookAty, lookAtz, upx, upy, upz);
  
  // Draw Buildings
  for (int i = 0; i < numBuildings; i++)
  {
    drawMesh(buildings[i]);
  }
  
  // Draw Streets
  for (int i = 0; i < numStreets; i++)
  {
    drawMesh(streets[i]);
  }
  
  // Draw vehicle
  drawMesh(vehicle);
  
  //Draw tanks
  for (int i = 0; i < num_of_tanks; i++) {
      tank[i]->draw();
  }
  
  terrainGrid->DrawGrid(gridSize);
  
  glutSwapBuffers();
}


// Called at initialization and whenever user resizes the window */
void reshape(int w, int h)
{
  glutWindowWidth = w; glutWindowHeight = h;
  
  viewportWidth  = glutWindowWidth;
  viewportHeight = glutWindowHeight;
  
  glViewport(0, 0, (GLsizei) viewportWidth, (GLsizei) viewportHeight);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  // keep same aspect ratio as screen window
  gluPerspective(60.0*zoomFactor,(float)viewportWidth/(float)viewportHeight,0.2,80.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

VECTOR3D pos = VECTOR3D(0,0,0);

// Mouse Control coordinates
int prevx, prevy;

int selectedControlPoint = -1;

void mouse(int button, int state, int x, int y)
{
  currentButton = button;
  
  switch(button)
  {
    case GLUT_LEFT_BUTTON:
      if (state == GLUT_DOWN)
      {
        prevx = x;
        prevy = y;
      }
      break;
      
    default:
      break;
  }
  
  glutPostRedisplay();
}


/**************************************************************************
 * Mouse Control
 **************************************************************************/

void mouseMotionHandler(int xMouse, int yMouse)
{
  if (currentButton == GLUT_LEFT_BUTTON)
  {
    VECTOR3D wpos;
    
    angleTheta += prevx - xMouse;
    anglePhi += prevy - yMouse;
    
    while (angleTheta < -180)
      angleTheta += 360;
    while (angleTheta > 180)
      angleTheta -= 360;
    while (anglePhi < 0)
      anglePhi += 360;
    
    limitCameraAngle();
    
  }
  else if (currentButton == GLUT_RIGHT_BUTTON)
  {
    if (yMouse < prevy) {
      if (radius < 35) {
        radius++;
      }
    } else {
      if (radius > 6) {
        radius--;
      }
    }
  }
  
  prevx = xMouse;
  prevy = yMouse;
  
  return;
}

/**************************************************************************
 * Timer function to limit Frames Per Second
 **************************************************************************/
void timer(int value)
{
  glutTimerFunc(1000.0 / FPS, timer, 0);
  animationFunction(10.0/FPS);
  glutPostRedisplay();
}


/**************************************************************************
 * Utility Functions
 **************************************************************************/
float degToRad(float degrees)
{
  return degrees / 180 * M_PI;
}

float radToDeg(float radians)
{
  return radians * 180 / M_PI;
}

void updateCameraPos()
{
  // Spherical coordinates formula
  lookFromx = lookAtx + radius * sin(anglePhi*0.0174532) * sin(angleTheta*0.0174532);
  lookFromy = lookAty + radius * cos(anglePhi*0.0174532);
  lookFromz = lookAtz + radius * sin(anglePhi*0.0174532) * cos(angleTheta*0.0174532);
}

float* calculateBoundingBox(BuildingMesh* mesh)
{
  float xmin, xmax, zmin, zmax;
  
  float scalefactor = mesh->scaleFactor.z;
  if (mesh->scaleFactor.x > mesh->scaleFactor.z) {
    scalefactor = mesh->scaleFactor.x;
  }
  
  xmin = mesh->translation.x - scalefactor;
  xmax = mesh->translation.x + scalefactor;
  zmin = mesh->translation.z - scalefactor;
  zmax = mesh->translation.z + scalefactor;
  
  float* bounds = (float*)malloc(4 * sizeof(float));
  bounds[0] = xmin; bounds[1] = xmax; bounds[2] = zmin; bounds[3] = zmax;
  
  return bounds;
}

bool checkForCollision(BuildingMesh* mesh1, BuildingMesh* mesh2)
{
  float* bounds1_ptr = calculateBoundingBox(mesh1);
  float* bounds2_ptr = calculateBoundingBox(mesh2);
  
  float bounds1[] = {bounds1_ptr[0], bounds1_ptr[1], bounds1_ptr[2], bounds1_ptr[3]};
  float bounds2[] = {bounds2_ptr[0], bounds2_ptr[1], bounds2_ptr[2], bounds2_ptr[3]};
  
  free(bounds1_ptr); free(bounds2_ptr);
  
  bool collision = false;
  float margin = 0.0;
  
  // xmin of mesh 1 <= xmax of mesh2 AND xmax of mesh 1 >= xmin of mesh 2
  if (bounds1[0] - margin < bounds2[1] && bounds1[1] + margin > bounds2[0])
    // zmin of mesh 1 <= zmax of mesh2 AND zmax of mesh 1 >= zmin of mesh 2
    if (bounds1[2] - margin < bounds2[3] && bounds1[3] + margin > bounds2[2])
      collision = true;  // collision detected
  
  return collision;
}

bool checkCollisionWithBuildings()
{
  bool collision = false;
  
  for (int i = 0; i < numBuildings; i++)
    if (checkForCollision(vehicle, buildings[i]))
      collision = true;
  
  return collision;
}


/**************************************************************************
 * Limit Camera angle
 **************************************************************************/
void limitCameraAngle()
{
  if (anglePhi > 80)
    anglePhi = 80;
  if (anglePhi < 30)
    anglePhi = 30;
}


VECTOR3D ScreenToWorld(int x, int y)
{
  
  GLint viewport[4];
  GLdouble modelview[16];
  GLdouble projection[16];
  GLfloat winX, winY, winZ;
  GLdouble posX, posY, posZ;
  
  glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
  glGetDoublev( GL_PROJECTION_MATRIX, projection );
  glGetIntegerv( GL_VIEWPORT, viewport );
  
  winX = (float)x;
  winY = (float)viewport[3] - (float)y;
  
  // Read all pixels at given screen XY from the Depth Buffer
  glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
  gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
  
  return VECTOR3D((float)posX, (float)posY, (float)posZ);
}


/* Handles input from the keyboard, non-arrow keys */
void keyboard(unsigned char key, int x, int y)
{
    
  double xtmp, ztmp, xnew, znew;
  switch (key)
  {
      // Navigate
    case 'n':
      if (currentAction != NAVIGATE)
      {
        currentAction = NAVIGATE;
        tank[selected_tank]->updateCamera();
          
        lookFromx = tank[selected_tank]->lookFrom.x;
        lookFromy = tank[selected_tank]->lookFrom.y;
        lookFromz = tank[selected_tank]->lookFrom.z;
          
        lookAtx = tank[selected_tank]->lookAt.x;
        lookAty = tank[selected_tank]->lookAt.y;
        lookAtz = tank[selected_tank]->lookAt.z;
          
        /*
        camerax = lookFromx;
        cameray = lookFromy;
        cameraz = lookFromz;
        ztmp = vehicle->scaleFactor.z+0.1;
        xnew = ztmp * sin (degToRad(vehicle->angles.y));
        znew = ztmp * cos (degToRad(vehicle->angles.y));
        lookFromx = vehicle->translation.x;
        lookFromy = 2*buildingFloorHeight;
        lookFromz = vehicle->translation.z;
        lookAtx = vehicle->translation.x - 2*xnew;
        lookAty = buildingFloorHeight;
        lookAtz = vehicle->translation.z - 2*znew; */
      }
      else
      {
        currentAction = TRANSLATE;
        lookFromx = camerax;
        lookFromy = cameray;
        lookFromz = cameraz;
      }
      break;
    case 'm':
          
          
        break;
    case 'w':
      glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
      break;
    case 'f':
      glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
      break;
  }
  glutPostRedisplay();
}

void functionUpKeys (int key, int x, int y) {
    currentFuncKey = -1;
}

void functionKeys(int key, int x, int y)
{
  double xtmp, ztmp, xnew, znew;
  float old_translationX, old_translationZ;
    
  currentFuncKey = key;
  /*
  if (currentAction == NAVIGATE)
  {
    old_translationX = vehicle->translation.x;
    old_translationZ = vehicle->translation.z;
    switch (key)
    {
      case GLUT_KEY_DOWN:
        vehicle->translation.x += 0.2 * sin (degToRad(vehicle->angles.y));
        vehicle->translation.z += 0.2 * cos (degToRad(vehicle->angles.y));
        if (checkCollisionWithBuildings()) {
          vehicle->translation.x = old_translationX;
          vehicle->translation.z = old_translationZ;
        }
        break;
      case GLUT_KEY_UP:
        vehicle->translation.x -= 0.2 * sin (degToRad(vehicle->angles.y));
        vehicle->translation.z -= 0.2 * cos (degToRad(vehicle->angles.y));
        if (checkCollisionWithBuildings()) {
          vehicle->translation.x = old_translationX;
          vehicle->translation.z = old_translationZ;
        }
        break;
      case GLUT_KEY_RIGHT:
        vehicle->angles.y -= 2.0;
        break;
        
      case GLUT_KEY_LEFT:
        vehicle->angles.y += 2.0;
        break;
    }
    ztmp = vehicle->scaleFactor.z+0.1;
    xnew = ztmp * sin (degToRad(vehicle->angles.y));
    znew = ztmp * cos (degToRad(vehicle->angles.y));
    lookFromx = vehicle->translation.x;
    lookFromy = 2*buildingFloorHeight;
    lookFromz = vehicle->translation.z;
    lookAtx = vehicle->translation.x - 2*xnew;
    lookAty = buildingFloorHeight;
    lookAtz = vehicle->translation.z - 2*znew;
  } */
}

void animationFunction (float delta_time) {
    static double ztmp, xnew, znew;
    static float old_translationX, old_translationZ;
    
    if (currentAction == NAVIGATE)
    {
        old_translationX = vehicle->translation.x;
        old_translationZ = vehicle->translation.z;
        switch (currentFuncKey)
        {
            case GLUT_KEY_DOWN:
                tank[selected_tank]->moveBy(-0.2 * delta_time);
                
                /*
                vehicle->translation.x += 0.2 * sin (degToRad(vehicle->angles.y)) * delta_time;
                vehicle->translation.z += 0.2 * cos (degToRad(vehicle->angles.y)) * delta_time;
                if (checkCollisionWithBuildings()) {
                    vehicle->translation.x = old_translationX;
                    vehicle->translation.z = old_translationZ;
                } */
                break;
            case GLUT_KEY_UP:
                 tank[selected_tank]->moveBy(0.2 * delta_time);
                /*
                vehicle->translation.x -= 0.2 * sin (degToRad(vehicle->angles.y)) * delta_time;
                vehicle->translation.z -= 0.2 * cos (degToRad(vehicle->angles.y)) * delta_time;
                if (checkCollisionWithBuildings()) {
                    vehicle->translation.x = old_translationX;
                    vehicle->translation.z = old_translationZ;
                } */
                break;
            case GLUT_KEY_RIGHT:
                tank[selected_tank]->rotateBy(-2.0 * delta_time);
              //  tank[selected_tank]->rotateCannon(-2.0 * delta_time);
                tank[selected_tank]->rotateTurret(-2.0 * delta_time);
               // vehicle->angles.y -= 2.0 * delta_time;
                break;
                
            case GLUT_KEY_LEFT:
                tank[selected_tank]->rotateBy(2.0 * delta_time);
                //tank[selected_tank]->rotateCannon(2.0 * delta_time);
                tank[selected_tank]->rotateTurret(2.0 * delta_time);
                //vehicle->angles.y += 2.0 * delta_time;
                break;
        }
        lookFromx = tank[selected_tank]->lookFrom.x;
        lookFromy = tank[selected_tank]->lookFrom.y;
        lookFromz = tank[selected_tank]->lookFrom.z;
        
        lookAtx = tank[selected_tank]->lookAt.x;
        lookAty = tank[selected_tank]->lookAt.y;
        lookAtz = tank[selected_tank]->lookAt.z;
        /*
        ztmp = vehicle->scaleFactor.z+0.1;
        xnew = ztmp * sin (degToRad(vehicle->angles.y));
        znew = ztmp * cos (degToRad(vehicle->angles.y));
        lookFromx = vehicle->translation.x;
        lookFromy = 2*buildingFloorHeight;
        lookFromz = vehicle->translation.z;
        lookAtx = vehicle->translation.x - 2*xnew;
        lookAty = buildingFloorHeight;
        lookAtz = vehicle->translation.z - 2*znew;*/
    }
}

void loadTank(Tank **tank_new){
    *tank_new = new Tank();
    
    
    load_obj(tank_fileName, &(*tank_new)->body);
    load_obj(cannon_fileName, &(*tank_new)->cannon);
    load_obj(turret_fileName, &(*tank_new)->turret);
    
    std::vector<ObjMesh *> * wheels = new std::vector<ObjMesh *>();
    ObjMesh *wheel;
    
    //Loads Wheels
    load_obj(wheel1_fileName, &wheel);
    wheels->push_back(wheel);
    wheel->use_center_x_translate = true;
    
    load_obj(wheel2_fileName, &wheel);
    wheels->push_back(wheel);
    wheel->use_center_x_translate = true;
    
    load_obj(wheel3_fileName, &wheel);
    wheels->push_back(wheel);
     wheel->use_center_x_translate = true;
    
    load_obj(wheel4_fileName, &wheel);
    wheels->push_back(wheel);
    wheel->use_center_x_translate = true;
    (*tank_new)->set_wheels(*wheels);
    
    (*tank_new)->body->setTextureMapID(2006);
    (*tank_new)->cannon->setTextureMapID(2007);
    (*tank_new)->turret->setTextureMapID(2008);
    (*tank_new)->wheels[0]->setTextureMapID(2009);
    (*tank_new)->wheels[1]->setTextureMapID(2009);
    (*tank_new)->wheels[2]->setTextureMapID(2009);
    (*tank_new)->wheels[3]->setTextureMapID(2009);
}

