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
#include "init.hpp"

class robot() {
    //default will create a simple cube
    robot() : robotTime(t) {
        masses = generateMasses(defaultMassWeight);
        springs = generateSprings(masses,springs);
        linkMassSpring(masses,springs);
    }
    // given objects
    robot(std::vector<mass>& m, std::vector<spring&> s);
    // copy
    robot (const spring& rhs);
    //assignment
    robot& operator=(const robot& right);
    
    // member functions
    void simulate(bool multicore = false, int maxSteps = 1, bool pulse = false);
    vector<double> centerOfMass();
    
    // function to randomize the spring values?
    void randomizeSprings();
    // function to add a mass somewhere? do we want that?
    void addRandomMass();
    // remove a random mass
    void removeRandomMass();
    // remove random spring
    void removeRandomSpring();
    // add random spring (this might not always add a spring if there is already one between the two random masses)
    // dont want to force it to try too long so instead will do nothing
    void addRandomSpring();
    
    
    
    // member variables
    // these should probably change to linked lists?
    vector<mass> masses;
    vector<spring> springs;
    double robotTime;
}



#endif /* robot_hpp */
