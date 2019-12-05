//
//  force.hpp
//  ECD-A3
//
//  Created by Salvy Cavicchio on 11/7/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#ifndef force_hpp
#define force_hpp

#include "mass.hpp"
#include "globalVars.h"
#include <iostream>

struct force {
    force(mass* b, const bool addAllForces = true) : body(b) {
        f[0] = 0; f[1] = 0; f[2] = 0;
        if (addAllForces) {
            this->addGravity();
            this->addSpringForce();
            this->addFrictionForce();
            this->addResultantForce();
        }
    }
    
    force (const force& rhs);
    force& operator=(const force& right);
    ~force() {    std::cout << "Destructed Force" << std::endl;
};
    
    void addSpringForce();
    void addGravity();
    void addFrictionForce();
    void addResultantForce();
    
    mass* body;
    double f[3];
};
#endif /* force_hpp */
