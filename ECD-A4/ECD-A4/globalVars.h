//
//  globalVars.h
//  ECD-A4
//
#ifndef globalVars_h
#define globalVars_h

#include "GLFW/glfw3.h"

static const GLfloat WHITE[] = {1, 1, 1};
static const GLfloat RED[] = {1, 0, 0};
static const GLfloat GREEN[] = {0, 1, 0};
static const GLfloat MAGENTA[] = {1, 0, 1};

extern const double timestep;
extern const double defaultWeight;
extern const double maxRobotMass;

extern const double defaultPhi;
extern const double defaultAmplitde;
extern const double defaultK;
extern const double g[3];
extern const double friction_mu_k;
extern const double kc;
extern const double w;
extern const double damping;
#endif /* globalVars_h */
