//
//  CannonRound.h
//  TankSim
//
//  Created by Matthew Tesfaldet on 2014-11-22.
//  Copyright (c) 2014 Matthew Tesfaldet. All rights reserved.
//

#ifndef TankSim_CannonRound_h
#define TankSim_CannonRound_h
#include <math.h>
#include "VECTOR3D.h"
#define RAD 3.14159265 / 180

class CannonRound {
  
public:
  VECTOR3D translation;
  VECTOR3D direction;
  VECTOR3D scaleFactor;
  
  // mesh (not used yet)
  ObjMesh *body;
  
  // other
  float distance_moved = 0;
    
  //for collision animation
    float live_for;
    float lived;
  
  
  CannonRound() {
    this->body = nullptr;
    this->live_for = 2;
    this->lived = 0;
  };
  
  void set_body (ObjMesh *body) {
    if (this->body) {
      free(this->body);
    }
    
    this->body = body;
    this->body->translation += this->translation;
  };
  
  void draw() {
      this->body->draw();
    
    // draw a glut sphere for now (don't use a blender object yet)
   // glPushMatrix();
   // glTranslatef(translation.x, translation.y, translation.z);
  //  glScalef(scaleFactor.x, scaleFactor.y, scaleFactor.z);
   // glutSolidSphere(0.2, 30, 30);
   // glPopMatrix();
  };
  
  /* Projects the bullet along a direction */
  void moveBy (float distance) {
    distance_moved += sqrt(distance * distance);
    
    float new_x  = distance * sinf(this->direction.y * RAD);
    float new_z  = distance * cosf(this->direction.y * RAD);
    float new_y  = 0.00;
    
    this->translation.x += new_x;
    this->translation.z += new_z;
    this->translation.y += new_y;
      
    this->body->translation = this->translation;
  };
    
  void addScale(VECTOR3D scale){
    this->body->scaleFactor += scale;
  }
    
  void resetBody() {
      this->body->translation = this->translation;
      this->body->scaleFactor = this->scaleFactor;
  }
};

#endif
