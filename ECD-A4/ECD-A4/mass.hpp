//
//  mass.hpp
//  ECD-A4
//
//  Created by Salvy Cavicchio on 12/5/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#ifndef mass_hpp
#define mass_hpp

#include <stdio.h>
#include <vector>
#include "globalVars.h"

struct mass {
    mass(double m = defaultWeight, double x = 0, double y = 0, double z = 0, bool fix = false) : weight(m), fixed(fix) {
        p[0] = x; p[1] = y; p[2] = z;
        for (int i = 0; i < 3; i++) {
            v[i] = 0;
            a[i] = 0;
            f[i] = 0;
        }
    }
    // copy constructor
    mass(const mass& rhs) {
        weight = rhs.weight;
        fixed = rhs.fixed;
        for (int i = 0; i < 2; i++) {
            p[i] = rhs.p[i];
            v[i] = rhs.v[i];
            a[i] = rhs.a[i];
        }
        return;
    };
    // assignment operator
    mass& operator=(const mass& rhs) {
        if (this != &rhs) {
            weight = rhs.weight;
            fixed = rhs.fixed;
            for (int i = 0; i < 2; i++) {
                p[i] = rhs.p[i];
                v[i] = rhs.v[i];
                a[i] = rhs.a[i];
            }
        }
        return *this;
    };
    // descrutros
    
    void resetForces() {
        f[0] = 0; f[1] = 0; f[2] = 0;
    };
    
    void updateDerivitives(const double d = damping) {
        if (!fixed) {
            for (int i = 0; i < 3; i++) {
                a[i] = f[i]/weight;
                v[i] += (a[i]*timestep)*d;
                p[i] += v[i]*timestep;
            }
        }
        return;
    }
    
    // members
    double weight;
    double p[3];
    double v[3];
    double a[3];
    double f[3];
    bool fixed;

};



#endif /* mass_hpp */
