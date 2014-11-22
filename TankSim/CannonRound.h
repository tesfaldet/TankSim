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

class CannonRound {
  
public:
  VECTOR3D translation;
  
  // mesh
  ObjMesh *body;
  
  CannonRound() {
    translation = NULL;
  };
  
  
};

#endif
