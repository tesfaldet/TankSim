//
//  Tank.h
//  TankSim
//
//  Created by Vitaliy Krasylovets on 2014-11-17.
//  Copyright (c) 2014 Matthew Tesfaldet. All rights reserved.
//

#ifndef TankSim_Tank_h
#define TankSim_Tank_h
#include <math.h>
#include "VECTOR3D.h"
#include "CannonRound.h"
#define RAD 3.14159265 / 180

class Tank {
    
    public:
    VECTOR3D translation;
    VECTOR3D angles; //angle around the Y axis
    
    //Meshes
    ObjMesh *body = nullptr;
    ObjMesh *(*wheels) = nullptr;
    ObjMesh *cannon = nullptr;
    ObjMesh *turret= nullptr;
    
    //Camera
    VECTOR3D lookFrom;
    VECTOR3D lookAt;
    
    //Other
    int num_of_wheels;
    float cam_height;
  
    CannonRound* round = nullptr;
    bool cannonFired = false;
    bool cannonHit = false;
  
    bool hit = false;
    
    
  
    Tank() {
        this->num_of_wheels = 0;
        this->cam_height = 1;
        this->round = new CannonRound();
    };
    
    
    void set_body (ObjMesh *body) {
        if (this->body) {
            free(this->body);
        }
        
        this->body = body;
        this->body->translation += this->translation;
    };
    
    void set_wheels (std::vector<ObjMesh *> &wheels) {
        if(this->wheels){
            free(this->wheels);
        }
        
        this->num_of_wheels = (int) wheels.size();
        this->wheels = &wheels[0];
        
        for(int i = 0; i < num_of_wheels; i++) {
            this->wheels[i]->translation.x += this->translation.x;
            this->wheels[i]->translation.y += this->translation.y;
            this->wheels[i]->translation.z += this->translation.z;
        }
    };
    
    void set_cannon (ObjMesh *cannon) {
        if (this->cannon) {
            free(this->cannon);
        }
        
        this->cannon = cannon;
        this->cannon->translation += this->translation;
    }
    
    void set_turret (ObjMesh *turret) {
        if (this->turret) {
            free(this->turret);
        }
        
        this->turret = turret;
        this->turret->translation += this->translation;
        
        updateCamera();
    }
    
    void draw() {
      if (this->hit) {
        this->body->mat_diffuse[0] = 1.0;
        this->body->mat_diffuse[1] = 0.0;
        this->body->mat_diffuse[2] = 0.0;
        this->cannon->mat_diffuse[0] = 1.0;
        this->cannon->mat_diffuse[1] = 0.0;
        this->cannon->mat_diffuse[2] = 0.0;
        this->turret->mat_diffuse[0] = 1.0;
        this->turret->mat_diffuse[1] = 0.0;
        this->turret->mat_diffuse[2] = 0.0;
      }
        this->body->draw();
        this->cannon->draw();
        this->turret->draw();
        
        for (int i = 0; i < num_of_wheels; i++) {
            this->wheels[i]->draw();
        }
      
        if (this->cannonFired || this->cannonHit) {
          this->round->draw();
        }
        
    };
    
    /*Moves the Tank by Vector */
    void moveBy (VECTOR3D position) {
        this->translation.x += position.x;
        this->translation.y += position.y;
        this->translation.z += position.z;
        
        this->body->translation.x += position.x;
        this->body->translation.y += position.y;
        this->body->translation.z += position.z;
        
        this->cannon->translation.x += position.x;
        this->cannon->translation.y += position.y;
        this->cannon->translation.z += position.z;
        
        this->turret->translation.x += position.x;
        this->turret->translation.y += position.y;
        this->turret->translation.z += position.z;
        
        for(int i = 0; i < num_of_wheels; i++) {
            this->wheels[i]->translation.x += position.x;
            this->wheels[i]->translation.y += position.y;
            this->wheels[i]->translation.z += position.z;
        }
        
        updateCamera();
    };
    
    /*Moves the Tank Forward or backwards */
    void moveBy (float distance) {
      
        int sign = distance < 0 ? 1 : -1;
      
        float new_x  = distance * sinf(this->angles.y * RAD);
        float new_z  = distance * cosf(this->angles.y * RAD);
        
        this->translation.x += new_x;
        this->translation.z += new_z;
        
        this->body->translation.x += new_x;
        this->body->translation.z += new_z;
        
        this->cannon->translation.x += new_x;
        this->cannon->translation.z += new_z;
        
        this->turret->translation.x += new_x;
        this->turret->translation.z += new_z;
        
        for(int i = 0; i < num_of_wheels; i++) {
            this->wheels[i]->translation.x += new_x;
            this->wheels[i]->translation.z += new_z;
            this->wheels[i]->angles.x += 5 * -sign;
        }
        updateCamera();
    };
    
    
    /* Rotates the Tanks
       Rotations doesn't affect the cannon or the turret
     */
    void rotateBy (float angle) {
        this->angles.y += angle;
        
        //Body transformation
        this->body->angles.y += angle;
        
        //Wheels transformation
        for (int i = 0; i < num_of_wheels; i++) {
            this->wheels[i]->angles.y += angle;
        }
    };
    
    /* Rotates the Tank Cannon */
    void rotateCannon (float angle) {
        this->cannon->angles.y += angle;
    }
    
    /* Rotates the Turret */
    void rotateTurret (float angle) {
        this->turret->angles.y += angle;
         updateCamera();
    };
    
    void rotateTank(float angle) {
        rotateBy (angle);
        rotateCannon (angle);
        rotateTurret (angle);
    }
  
    void fireCannon() {
      if (!this->cannonFired) {
        this->cannonHit = false;
        this->cannonFired = true;
        this->round->direction = this->cannon->angles;
        this->round->translation = this->cannon->translation;
        this->round->scaleFactor = VECTOR3D(0.2,0.2,0.2);
        this->round->distance_moved = 0;
        
        // initializing position of round to be in front of cannon
        this->round->translation.y = 0.3;
        this->round->translation.x += 1.5 * sinf(this->round->direction.y * RAD);
        this->round->translation.z += 1.5 * cosf(this->round->direction.y * RAD);
          
        this->round->resetBody();
      }
    }
  
    void animateRound() {
      if (this->round->distance_moved < 40 && !this->cannonHit) {
        this->round->moveBy(0.5);
      } else if (this->cannonHit) {
        this->round->addScale(VECTOR3D(0.5,0.5,0.5));
      } else {
        this->round->body->setTextureMapID(-1);
        this->cannonFired = false;
        this->cannonHit = false;
      }
    }
  
    /* Updates the Camera Position */
    void updateCamera() {
        
        float xnew  = sinf(this->turret->angles.y * RAD);
        float znew  = cosf(this->turret->angles.y * RAD);
        
        //Assign look from Point
        this->lookFrom.x = this->cannon->translation.x;
        this->lookFrom.y = this->cannon->translation.y + this->cam_height;
        this->lookFrom.z = this->cannon->translation.z;
        
        //Assign look at Point
        this->lookAt.x = this->lookFrom.x + (2 * xnew);
        this->lookAt.y = this->cam_height -1;
        this->lookAt.z = this->lookFrom.z + (2 * znew);
    }
    
};

#endif
