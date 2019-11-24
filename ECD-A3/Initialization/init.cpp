//
//  init.cpp
//  ECD-A3
//
//  Created by Salvy Cavicchio on 11/13/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#include "init.hpp"

std::vector<mass> generateMasses(double weight) {
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

std::vector<spring>& generateSprings(double k, std::vector<mass> &masses, std::vector<spring>& theResult) {
    for (size_t i = 0; i < masses.size(); i++) {
        for (size_t j = i; j < masses.size(); j++) { // starts with i to avoid duplicates
            if (j != i) {             // if they are the same we will skip
                spring aSpring(k,&masses[i],&masses[j]);
                theResult.push_back(aSpring);
            }}}
    /*for (spring item : theResult) {
        item.m1->linkSpring(&item);
        item.m2->linkSpring(&item);
    }*/
    return theResult;
}

void linkMassSpring(std::vector<mass>& masses, std::vector<spring>& springs) {
    for (std::vector<spring>::iterator s = springs.begin(); s != springs.end(); s++) {
        for (std::vector<mass>::iterator m = masses.begin(); m != masses.end(); m++) {
            if (&(*m) == (*s).m1) { (*m).s.push_back(&(*s)); }
            else if (&(*m) == (*s).m2) { (*m).s.push_back(&(*s)); }
        }
    }
}
