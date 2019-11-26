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
    robot();
    // given objects
    robot(std::vector<mass>& m, std::vector<spring&> s);
    // copy
    robot (const spring& rhs);
    //assignment
    robot& operator=(const robot& right);
    
    // member functions
    void simulate(bool multicore = false, int maxSteps = 1, bool pulse = false);
    vector<double> centerOfMass();
    
    // member variables
    // these should probably change to linked lists?
    vector<mass> masses;
    vector<spring> springs;
}



#endif /* robot_hpp */
