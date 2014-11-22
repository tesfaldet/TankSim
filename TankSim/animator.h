//
//  animator.h
//  TankSim
//
//  Created by Vitaliy Krasylovets on 2014-11-22.
//  Copyright (c) 2014 Matthew Tesfaldet. All rights reserved.
//

#ifndef TankSim_animator_h
#define TankSim_animator_h

#include <cmath>
#include "Tank.h"

class Animator {
        bool driveBack;
        bool rotated;
    public:
        Tank *tank;
        float distance = 0;
        float distance_moved = 0;
        float angle = 0;
        float angle_moved = 0;
    
    Animator(Tank *tank, float distance, float angle) :
        tank(tank), distance(distance), angle(angle) {
            this->angle_moved = 0;
            this->distance_moved = 0;
            this->driveBack = false;
            this->rotated = false;
    };
    
    void animate(float distance, float angle) {
        if (!this->driveBack && !this->rotated) {
            this->tank->moveBy(distance);
            this->distance_moved += std::abs(distance);
            if (this->distance_moved >= this->distance) {
                this->distance_moved = 0;
                this->driveBack = true;
            }
            
        } else if (this->driveBack && !this->rotated) {
            this->tank->rotateTank(angle);
            this->angle_moved += angle;
            if (this->angle_moved >= this->angle) {
                this->angle_moved = 0;
                this->rotated = true;
            }
        } else if (this->driveBack && this->rotated) {
            this->tank->moveBy(distance);
            this->distance_moved += std::abs(distance);
            if (this->distance_moved >= this->distance) {
                this->distance_moved = 0;
                this->driveBack = false;
            }
        } else if (!this->driveBack && this->rotated) {
            this->tank->rotateTank(angle);
            this->angle_moved += angle;
            if (this->angle_moved >= this->angle) {
                this->angle_moved = 0;
                this->rotated = false;
            }
        }
    }
};

#endif
