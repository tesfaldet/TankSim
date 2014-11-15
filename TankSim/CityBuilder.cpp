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

#include "TerrainGrid.h"
#include "Mesh.h"

void initOpenGL();
void display(void);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseMotionHandler(int xMouse, int yMouse);
void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
void timer(int value);
VECTOR3D ScreenToWorld(int x, int y);
void updateCameraPos();
void limitCameraAngle();

static int currentButton;
static unsigned char currentKey;
const float FPS = 30.0;

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
Mesh *streets[10];
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
  trans.x = -6.0;
  trans.z = -6.0;
  trans.y = 0;

  buildings[0] = createMesh(scale, trans, buildingFloorHeight, numFloors);
  numBuildings = 1;

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
  numStreets = 1;

  // Create a vehicle - initially oriented along z axis direction
  scale.x = 0.125;
  scale.z = 0.25;
  scale.y = 1.0;
  trans.x = -4.0;
  trans.z = 6.0;
  trans.y = 0;

  vehicle = createMesh(scale, trans, 0.125, 2);
  vehicle->angles.x = vehicle->angles.y = vehicle->angles.z = 0.0; 
  vehicle->selected = true;
}


void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
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
  for (int i = 0; i < numBuildings; i++)
  {
	  drawMesh(streets[i]);
  }
  // Draw vehicle
  drawMesh(vehicle);

  terrainGrid->DrawGrid(gridSize);
    
  /*
  if (GetAsyncKeyState(VK_END))
  {
	radius++;
	//zoomFactor += 0.02;
  }
  if (GetAsyncKeyState(VK_HOME))
  {
	  if (radius > 6)
		radius--;
	  //zoomFactor -= 0.02;
  } */
    
  // Zoom by changing view frustum
  /*
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0*zoomFactor,(float)width/(float)height,0.2,80.0);
    glMatrixMode(GL_MODELVIEW);
  */
  
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


/**************************************************************************
 * Limit Camera angle
 **************************************************************************/
void limitCameraAngle()
{
	if (anglePhi > 80)
		anglePhi = 80;
	if (anglePhi < 30)
		anglePhi = 30;
	//if (angleTheta < 10)
		//angleTheta = 10;
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
}// ScreenToWorld()


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
	      lookAtz = vehicle->translation.z - 2*znew;
	  }
	  else
	  {
		  currentAction = TRANSLATE;
		  lookFromx = camerax;
		  lookFromy = cameray;
		  lookFromz = cameraz;
	  }
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

void functionKeys(int key, int x, int y)
{
  double xtmp, ztmp, xnew, znew; 

  if (currentAction == NAVIGATE)
  {
	switch (key)
	{
	case GLUT_KEY_DOWN:
		vehicle->translation.x += 0.2 * sin (degToRad(vehicle->angles.y));
		vehicle->translation.z += 0.2 * cos (degToRad(vehicle->angles.y));
		break;
	case GLUT_KEY_UP:
		vehicle->translation.x -= 0.2 * sin (degToRad(vehicle->angles.y));
		vehicle->translation.z -= 0.2 * cos (degToRad(vehicle->angles.y));
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
  }
}

