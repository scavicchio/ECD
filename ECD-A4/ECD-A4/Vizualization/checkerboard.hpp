//
//  checkerboard.hpp
//  ECD-A4
//
//  Created by Tara Atkinson on 12/6/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#ifndef checkerboard_hpp
#define checkerboard_hpp

#include <stdio.h>
#include <GLUT/GLUT.h>
//#include <GLFW/glfw3.h>
#include "globalVars.h"

// Checkerboard Class
class Checkerboard {
  int displayListId;
  int width;
  int depth;
public:
  Checkerboard(int width, int depth): width(width), depth(depth) {}
    double centerx();
    double centerz();
    void create();
    void draw();
};


#endif /* checkerboard_hpp */

