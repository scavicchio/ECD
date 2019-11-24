//
//  Header.h
//  ECD-A3
//
//  Created by Salvy Cavicchio on 11/7/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#ifndef Header_h
#define Header_h

#include <stdio.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <stdlib.h>
#include "mass.hpp"
#include "spring.hpp"
#include "force.hpp"
#include "init.hpp"
#include <cmath>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

static GLfloat WHITE[] = {1, 1, 1};
static GLfloat RED[] = {1, 0, 0};
static GLfloat GREEN[] = {0, 1, 0};
static GLfloat MAGENTA[] = {1, 0, 1};

const double timestep = 0.00005;


#endif /* Header_h */
