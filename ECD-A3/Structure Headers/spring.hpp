//
//  spring.hpp
//  ECD-A3
//
//  Created by Salvy Cavicchio on 11/7/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#ifndef spring_hpp
#define spring_hpp

#include "mass.hpp"
#include "Header.h"
struct mass;

struct spring {
    spring(double stiffness, mass* mA, mass* mB, double b_init = .1, double c_init = 0) : k(stiffness), m1(mA), m2(mB), b(b_init), c(c_init) {
        orinLen = length();
    }
    spring (const spring& rhs);
    spring& operator=(const spring& right);
    
    friend std::ostream& operator<<(std::ostream& os, const spring& dt);
    
    double calcCurrentSpringForce(const bool pulse = false);
    const double length() const;
    const void display();
    void draw();
    const double pulseLength();
    
    double k;
    double orinLen;
    double b;
    double c;
    mass* m1;
    mass* m2;

};

#endif /* spring_hpp */
