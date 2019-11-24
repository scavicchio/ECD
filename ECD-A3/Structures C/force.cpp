//
//  force.cpp
//  ECD-A3
//
//  Created by Salvy Cavicchio on 11/7/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#include "force.hpp"
#include "mass.hpp"
#include <vector>
#include "spring.hpp"
#include <cmath>

force& force::operator=(const force& right) {
    if (this != &right) {
        body = right.body;
        for (int i = 0; i < 3; i++) {
            f[i] = right.f[i];
        }
    }
    return *this;
}

force::force(const force& rhs) {
    body = rhs.body;
    for (int i = 0; i < 3; i++) {
        f[i] = rhs.f[i];
    }
    return;
}

void force::addSpringForce(bool pulse) {
    for (std::vector<spring*>::iterator iter = body->s.begin(); iter != body->s.end(); iter++) {
        double di[3];
        spring* s = *iter;
        if (body == s->m1) {
            for (int i = 0; i < 3; i++) {
                di[i] = -(s->m1->p[i]-s->m2->p[i]);
            }
        }
        else {
            for (int i = 0; i < 3; i++) {
                di[i] = (s->m1->p[i]-s->m2->p[i]);                
            }
        }
        double totalD = sqrt(pow(di[0],2)+pow(di[1],2)+pow(di[2],2));
        double springF = s->calcCurrentSpringForce(pulse);
        for (int i = 0; i < 3; i++) {
            f[i] += springF*di[i]/totalD;
        }
    }
    return;
}

void force::addGravity() {
    for (int i = 0; i < 3; i++) {
        f[i] += (body->m)*g[i];
    }
    return;
}

void force::addFrictionForce() {
    if (body->p[1] < 0) {
   // horizontal force on mass
    double fH = sqrt(pow(f[0],2)+pow(f[2],2));
   // normal force on mass (with friciton factor
    double fN = f[1]+(body->m*g[1])*friction_mu_s;
    // do comparisons
    if (fH < fN) {
        f[0] = 0;
        f[2] = 0;
        }
    else {
        f[0] -= fN*friction_mu_s;
        f[2] -= fN*friction_mu_s;
        }
    }
    return;
}

void force::addResultantForce() {
    if (body->p[1] < 0) {
        // for bouncing on the ground
        f[1] -= kc*(body->p[1]);
    }
    return;
}
