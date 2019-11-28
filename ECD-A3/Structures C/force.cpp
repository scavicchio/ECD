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

std::vector<double> force::getSingleSpringForce(spring* s, bool pulse) {
   double di[3];
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
    std::vector<double> theReturn = {0,0,0};
    for (int i = 0; i < 3; i++) {
        theReturn[i] = springF*di[i]/totalD;
    }
    return theReturn;
}


void force::addSpringForce(bool pulse) {
    for (std::vector<spring*>::iterator iter = body->s.begin(); iter != body->s.end(); iter++) {
        spring* s = *iter;
        double di[3];
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
         std::vector<double> theReturn = {0,0,0};
         for (int i = 0; i < 3; i++) {
             f[i] = springF*di[i]/totalD;
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
    // will only add friction force if the body is on (or below) the ground AND the normal force is < 0
    if (body->p[1] <= 0 && f[1] < 0) {
        // a friction component is calculated as f_i = fN*u
        double fN = f[1]*friction_mu_k;
    //    double fH = sqrt(pow(f[0],2)+pow(f[2],2));
        if (f[0] < fN) { // then zero the velocity (ie the friction is slowing the mass)
            body->v[0] = 0;
        }
        else if (f[0] > 0) { f[0] -= fN; }
        else { f[0] += fN; }
        
        if (f[2] < fN) { // then zero the velocity (ie the friction is slowing the mass)
            body->v[2] = 0;
        }
        else if (f[2] > 0) { f[2] -= fN; }
        else { f[2] += fN; }
    }
    return;
}

void force::addResultantForce() {
    if (body->p[1] < 0) {
        // for bouncing on the ground
  //      body->p[1] = 0;
    //    body->v[1] = 0;
      //  body->a[1] = 0;
        f[1] -= kc*(body->p[1]);
    }
    return;
}
