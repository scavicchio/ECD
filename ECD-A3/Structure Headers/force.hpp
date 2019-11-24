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

const double g[3] = {0,-9.81,0};
const double kc = 1000000;

struct force {
    force(mass* b) : body(b) {
        f[0] = 0; f[1] = 0; f[2] = 0;
        this->addGravity();
        this->addResultantForce();
        this->addSpringForce();
    }
    
    force (const force& rhs);
    force& operator=(const force& right);
    
    void addSpringForce(bool print = false);
    void addGravity();
    void addResultantForce();
    
    mass* body;
    double f[3];
};
#endif /* force_hpp */
