//
//  mass.hpp
//  ECD-A3
//
//  Created by Salvy Cavicchio on 11/7/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#ifndef mass_hpp
#define mass_hpp

#include <vector>
#include <iostream>
/*
Each mass object needs the following attributes: m(mass, [kg]); p(3D position vector [meter]), v(3D velocityvector, meter/s),a(3D accelerationvector, meters/s2)
*/
struct force;
struct spring;
struct robot;

struct mass {
    mass(double& m, double x, double y, double z, const robot* rt, bool f = false) : m(m), fixed(f), parent(rt) {
           p[0] = x; p[1] = y; p[2] = z;
           for (int i = 0; i<3; i++) { v[i] = 0; a[i] = 0; }
       }
    
    mass(const mass& rhs);
    mass& operator=(const mass& right);
    ~mass() {
       // s.clear();
        parent = nullptr;
        std::cout << "Destructed Mass" << std::endl;
    };
    
    const void printP();
    void printV();
    void printA();
    void printSprings();
    void display();
    
    void updateDerivitives(const force& f,const double damping = 1);
    void moveMass(const double dx, const double dy, const double dz);
    //void linkSpring(spring* aSpring);
    force calcForce(bool pulse = false);
    void draw();
    
    double m;
    double p[3];
    double v[3];
    double a[3];
    bool fixed;
    std::vector<spring*> s;
    const robot* parent;
};

void drawSphere(double r, int lats, int longs);

#endif /* mass_hpp */
