//
//  taraRNG.cpp
//  ECD-A3
//
//  Created by Tara Atkinson on 11/26/19.
//  Copyright © 2019 Salvy. All rights reserved.
//
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>



double A = 0.1;

double rand_C()
{
    static std::random_device e;
    static std::uniform_real_distribution<> dis(0, 6.283185307);
    return dis(e);
}

double rand_B() {
    static std::random_device e;
    static std::uniform_real_distribution<> dis(0, 1);
    return dis(e);
}

double rand_K() {
    static std::random_device e;
    static std::uniform_real_distribution<> dis(1, 100000);
    return dis(e);
}
double rand_nummy() {
    static std::random_device e;
    static std::uniform_real_distribution<>dis(0,5);
    return dis(e);
}
double rand_pos() {
    static std::random_device e;
    static std::uniform_real_distribution<>dis(0,1);
    return dis(e);
}
