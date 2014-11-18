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
    ObjMesh *(*wheels) = nullptr;
    ObjMesh *cannon = nullptr;
    ObjMesh *turret= nullptr;
    
    //Camera
    VECTOR3D lookFrom;
    VECTOR3D lookAt;
    
    //Other
    int num_of_wheels;
    float cam_height;
    
    Tank() {
        this->num_of_wheels = 0;
        this->cam_height = 2;
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
        
        updateCamera(0);
    }
    
    void draw() {
        this->body->draw();
        this->cannon->draw();
        this->turret->draw();
        
        for (int i = 0; i < num_of_wheels; i++) {
            this->wheels[i]->draw();
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
        
        updateCamera(0);
    };
    
    /*Moves the Tank Forward of backwards */
    void moveBy (float distance) {
        float new_x = distance * sin(this->angles.y * RAD);
        float new_z = distance * cos(this->angles.z * RAD);
        
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
            this->wheels[i]->angles.z += 5;
        }
        updateCamera(0);
    };
    
    
    /* Rotates the Tanks
       Rotations doesn't affect the cannon or the turret
     */
    void rotateBy (float angle) {
        this->angles.y += angle;
        
        //Body transformation
        this->body->angles.y += this->angles.y;
        
        //Wheels transformation
        for (int i = 0; i < num_of_wheels; i++) {
            this->wheels[i]->angles.y += this->angles.y;
        }
    };
    
    /* Rotates the Tank Cannon */
    void rotateCannon (float angle) {
        this->cannon->angles.y += angle;
    }
    
    /* Rotates the Turret */
    void rotateTurret (float angle) {
        this->turret->angles.y += angle;
        this->updateCamera(angle);
    }
    
    /* Updates the Camera Position */
    void updateCamera(float angle) {
        float xnew = sin (angle * RAD);
        float znew = cos (angle * RAD);
        
        //Assign look from Point
        this->lookFrom = this->cannon->translation;
        this->lookFrom.y += this->cam_height;
        
        //Assign look at Point
        this->lookAt.x = this->lookFrom.x - 2 * xnew;
        this->lookAt.y = this->lookFrom.y - this->cam_height;
        this->lookAt.z = this->lookFrom.z - 2 * znew;
    }
    
};

#endif
