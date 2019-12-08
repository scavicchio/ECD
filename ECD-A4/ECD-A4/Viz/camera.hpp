//
//  camera.hpp
//  ECD-A4
//
//  Created by Tara Atkinson on 12/6/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#ifndef camera_hpp
#define camera_hpp

#include <stdio.h>

// Camera Class
class Camera {
  double theta;      // determines the x and z positions
  double y;          // the current y position
  double dTheta;     // increment in theta for swinging the camera around
  double dy;         // increment in y for moving the camera up/down
public:
  Camera(): theta(0.5), y(1), dTheta(0.04), dy(0.2) {}
    double getX();
    double getY();
    double getZ();
    void moveUp();
    void moveDown();
    void moveRight();
    void moveLeft();
};

#endif /* camera_hpp */
