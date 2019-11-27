//
//  robot.cpp
//  ECD-A3
//
//  Created by Salvy Cavicchio on 11/26/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#include "robot.hpp"
#include "init.hpp"
#include <random>
#include <ctime>

void robot::simulate(bool multicore, int maxSteps, bool pulse) {
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
            item->updateDerivitives(forces[i],damping);
            i++;
        }
        robotTime = robotTime+=timestep;
        maxSteps--;
    }
    return;
}

std::vector<double> robot::centerOfMass() {
    std::vector<double> topHalf = {0,0,0};
    double bottomHalf = 0;
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


std::vector<mass> robot::generateMasses(double weight) {
    mass m1(weight,0,0,0);
    mass m2(weight,0,.1,0);
    mass m3(weight,0,0,.1);
    mass m4(weight,0,.1,.1);
    mass m5(weight,.1,0,0);
    mass m6(weight,.1,.1,0);
    mass m7(weight,.1,0,.1);
    mass m8(weight,.1,.1,.1);
    std::vector<mass> theResult = {m1,m2,m3,m4,m5,m6,m7,m8};
    return theResult;
}
std::vector<spring> robot::generateSprings(double stiffness) {
    std::vector<spring> theResult;
     for (size_t i = 0; i < masses.size(); i++) {
           for (size_t j = i; j < masses.size(); j++) { // starts with i to avoid duplicates
               if (j != i) {             // if they are the same we will skip
                   spring aSpring(stiffness,&masses[i],&masses[j]);
                   theResult.push_back(aSpring);
               }}}
    return theResult;
}

void robot::linkMassSpring() {
    for (std::vector<spring>::iterator s = springs.begin(); s != springs.end(); s++) {
           for (std::vector<mass>::iterator m = masses.begin(); m != masses.end(); m++) {
               if (&(*m) == (*s).m1) { (*m).s.push_back(&(*s)); }
               else if (&(*m) == (*s).m2) { (*m).s.push_back(&(*s)); }
           }
       }
    return;
}


void robot::draw() {
    for (std::vector<mass>::iterator item = masses.begin(); item != masses.end(); item++) {
        item->draw();
    }
    for (std::vector<spring>::iterator item = springs.begin(); item != springs.end(); item++) {
        item->draw();
    }
    return;
}

double robot::generateRandom(const double range_start, const double range_end) {
    std::random_device e;
    std::uniform_real_distribution<> dis(range_start, range_end);
    return dis(e);
}

double robot::generateRandomPercentage(const double range_start, const double range_end) {
    std::random_device e;
    std::uniform_real_distribution<> dis(range_start, range_end);
    return dis(e);
}

void robot::randomizeSprings() {
    for (std::vector<spring>::iterator item = springs.begin(); item != springs.end(); item++) {
        item->b = generateRandomPercentage(0.8,1.2);
        item->c *= generateRandomPercentage();
        item->k *= generateRandomPercentage();
    }
}

void robot::reset() {
    masses[0].p[0] = 0; masses[0].p[1] = 0 ;masses[0].p[2] = 0;
    masses[1].p[0] = 0; masses[1].p[1] = .1 ;masses[1].p[2] = 0;
    masses[2].p[0] = 0; masses[2].p[1] = 0 ;masses[2].p[2] = .1;
    masses[3].p[0] = 0; masses[3].p[1] = .1 ;masses[3].p[2] = .1;
    masses[4].p[0] = .1; masses[4].p[1] = 0 ;masses[4].p[2] = 0;
    masses[5].p[0] = .1; masses[5].p[1] = .1 ;masses[5].p[2] = 0;
    masses[6].p[0] = .1; masses[6].p[1] = 0 ;masses[6].p[2] = .1;
    masses[7].p[0] = .1; masses[7].p[1] = .1 ;masses[7].p[2] = .1;
    
    for (mass& m : masses) { m.moveMass(0, .01, 0); }
    
    for (std::vector<mass>::iterator item = masses.begin(); item != masses.end(); item++) {
        item->v[0] = 0; item->v[1] = 0; item->v[2] = 0;
        item->a[0] = 0; item->a[1] = 0; item->a[2] = 0;
    }
    robotTime = 0;
    return;
}
