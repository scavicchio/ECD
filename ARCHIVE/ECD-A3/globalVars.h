//
//  globalVars.h
//  ECD-A3
//
//  Created by Salvy Cavicchio on 11/24/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#ifndef globalVars_h
#define globalVars_h
#include <GLFW/glfw3.h>

static const GLfloat WHITE[] = {1, 1, 1};
static const GLfloat RED[] = {1, 0, 0};
static const GLfloat GREEN[] = {0, 1, 0};
static const GLfloat MAGENTA[] = {1, 0, 1};

extern const double timestep;

extern double t;
extern double w;
extern double c;
extern const double damping;
extern const double friction_mu_s; // friction coefficient rubber-concrete
extern const double friction_mu_k; // friction coefficient rubber-concrete
extern const double k_vertices_soft; // spring constant of the edges
extern const double kc;
extern const double g[3];
extern double k;
extern const double defaultMassWeight; 

#endif /* globalVars_h */
