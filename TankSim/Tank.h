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
#define RAD 3.14159265/180

class Tank {
    
    public:
    VECTOR3D translation;
    VECTOR3D angles; //angle around the Y axis
    
    //Meshes
    ObjMesh *body = nullptr;
    ObjMesh *(*Wheels) = nullptr;
    ObjMesh *cannon = nullptr;
    ObjMesh *turret= nullptr;
    
    //Camera
    VECTOR3D lookFrom;
    VECTOR3D lookAt;
    
    //Other
    int num_of_wheels;
    
    Tank() {
        this->num_of_wheels = 0;
    };
    
    void draw() {
        if (this->body) {
            this->body->draw();
        }
        
        for (int i = 0; i < num_of_wheels; i++) {
            this->Wheels[i]->draw();
        }
        
        if (this->cannon) {
            this->cannon->draw();
        }
        
        if (turret) {
            this->turret->draw();
        }
    };
    
    void moveTo(VECTOR3D position) {
        this->translation.x = position.x;
        this->translation.y = position.y;
        this->translation.z = position.z;
        
        body->translation.x += this->translation.x;
        body->translation.y += this->translation.y;
        body->translation.z += this->translation.z;
        
        this->cannon->translation.x += this->translation.x;
        this->cannon->translation.y += this->translation.y;
        this->cannon->translation.z += this->translation.z;
    }
    
    /*Moves the Tank  */
    void moveBy (float distance) {
        this->translation.x += distance * sin(this->angles.y);
        this->translation.z += distance * cos(this->angles.z);
        
        //Body transformation
        body->translation.x += this->translation.x;
        body->translation.z += this->translation.z;
        
        //Wheels transformation
        for (int i = 0; i < num_of_wheels; i++) {
            this->Wheels[i]->translation.x += this->translation.x;
            this->Wheels[i]->translation.z += this->translation.z;
        }
        
        //Cannon transformation
        this->cannon->translation.x += this->translation.x;
        this->cannon->translation.z += this->translation.z;
        
        
        this->turret->translation.x += this->translation.x;
        this->turret->translation.z += this->translation.z;
    }
    
    
    /* Rotates the Tanks 
       Rotations doesn't affect the cannon or the turret
     */
    void rotateBy (float angle) {
        this->angles.y += angle;
        
        //Body transformation
        body->angles.y += this->angles.y;
        
        //Wheels transformation
        for (int i = 0; i < num_of_wheels; i++) {
            this->Wheels[i]->angles.y += this->angles.y;
        }
    }
    
    /* Rotates the Tank Cannon */
    void rotateCannon (float angle) {
        this->cannon->angles.y += angle;
    }
};

#endif
