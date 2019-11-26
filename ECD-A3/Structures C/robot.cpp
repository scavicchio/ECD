//
//  robot.cpp
//  ECD-A3
//
//  Created by Salvy Cavicchio on 11/26/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#include "robot.hpp"
#include <vector>

void robot::simulate(bool multicore = false, int maxSteps = 1, bool pulse = false) {
    std::vector<force> forces;
    
    while (maxSteps > 0) {
         forces.clear();
         // get the forces
        // this can become parallel later
        for (std::vector<mass>::iterator item = masses.begin(); item != masses.end(); item++) {
            forces.push_back(force(&(*item),pulse));
        }
        int i = 0;
        // this can also become parallel later
        for (std::vector<mass>::iterator item = masses.begin(); item != masses.end(); item++) {
            item->updateDerivitives(forces[i]);
            i++;
        }
        robotTime = robotTime+=timestep;
        maxSteps--;
    }
    return;
}

std::vector<double> robot::centerOfMass() {
    std::vector<double> topHalf = {0,0,0};
    double bottomHalf;
    std::vector<double> theReturn = {0,0,0};
    for (mass& item : masses) {
        topHalf[0] += item.m*item.p[0];
        topHalf[1] += item.m*item.p[1];
        topHalf[2] += item.m*item.p[2];
        bottomHalf += item.m;
    }
    for (int i = 0; i < 3; i++) { theReturn[i] = topHalf[i]/bottomHalf; }
    return theReturn;
}
