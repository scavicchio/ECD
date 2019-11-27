//
//  robot.hpp
//  ECD-A3
//
//  Created by Salvy Cavicchio on 11/26/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#ifndef robot_hpp
#define robot_hpp

#include <stdio.h>
#include "mass.hpp"
#include "force.hpp"
#include "spring.hpp"
#include "globalVars.h"

struct robot {
    //default will create a simple cube
    robot(bool p = false) : robotTime(t), pulse(p) {
        masses = generateMasses();
        springs = generateSprings();
        linkMassSpring();
    }
    // given objects
    // --------
    // copy
    robot (const spring& rhs);
    //assignment
    robot& operator=(const robot& right);
    
    // member functions
    void simulate(bool multicore = false, int maxSteps = 1, bool pulse = false);
    std::vector<double> centerOfMass();
    
    std::vector<mass> generateMasses(double weight = defaultMassWeight);
    std::vector<spring> generateSprings(double stiffness = k);
    void linkMassSpring();

    // function to randomize the spring values?
    void randomizeSprings();
    // function to add a mass somewhere? do we want that?
   /* void addRandomMass();
    // remove a random mass
    void removeRandomMass();
    // remove random spring
    void removeRandomSpring();
    // add random spring (this might not always add a spring if there is already one between the two random masses)
    // dont want to force it to try too long so instead will do nothing
    void addRandomSpring();*/
    
    void draw();
    double generateRandom(const double range_start, const double range_end);
    double generateRandomPercentage(const double range_start = 0.5, const double range_end = 1.5);
    void equalize(double threshold = 0.01, int depth = 0, int maxDepth = 10); // this is to zero out the forces before a simlation starts
    void reset();
    double calcMaxSpringForce();
    void setOnGround();

    // member variables
    // these should probably change to linked lists?
    std::vector<mass> masses;
    std::vector<spring> springs;
    double robotTime;
    bool pulse;
};


#endif /* robot_hpp */
