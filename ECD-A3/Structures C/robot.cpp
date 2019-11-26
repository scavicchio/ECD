//
//  robot.cpp
//  ECD-A3
//
//  Created by Salvy Cavicchio on 11/26/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#include "robot.hpp"

void robot::simulate(bool multicore = false, int maxSteps = 1, bool pulse = false) {
    vector<force> forces;
    
    while (maxSteps > 0) {
         forces.clear();
         // get the forces
        // this can become parallel later
        for (vector<mass>::iterator item = masses.begin(); item != masses.end(); item++) {
            forces.push_back(force(&(*item),pulse));
        }
        int i = 0;
        // this can also become parallel later
        for (vector<mass>::iterator item = masses.begin(); item != masses.end(); item++) {
            item->updateDerivitives(forces[i]);
            i++;
        }
        t = t+=timestep;
        maxSteps--;
    }
    return;
}