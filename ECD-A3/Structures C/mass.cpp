//
//  mass.cpp
//  ECD-A3
//
//  Created by Salvy Cavicchio on 11/7/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#include "mass.hpp"
#include "force.hpp"
#include "spring.hpp"
#include <iostream>
#include <GLUT/GLUT.h>
#include <cmath>
#include "globalVars.h"

mass& mass::operator=(const mass& right) {
    if (this != &right) {
        m = right.m;
        fixed = right.fixed;
        for (int i = 0; i < 3; i++) {
            p[i] = right.p[i];
            v[i] = right.v[i];
            a[i] = right.a[i];
        }
        s.clear();
        for (spring* item : right.s) {
            s.push_back(item);
        }
    }
    return *this;
}

mass::mass(const mass& rhs) {
    m = rhs.m;
    fixed = rhs.fixed;
    for (int i = 0; i < 3; i++) {
        p[i] = rhs.p[i];
        v[i] = rhs.v[i];
        a[i] = rhs.a[i];
    }
    s.clear();
    for (spring* item : rhs.s) {
        s.push_back(item);
    }
}

void mass::updateDerivitives(const force& f,const double damping) {
    if (!fixed) {
        for (int i = 0; i < 3; i++) {
            a[i] = f.f[i]/m;
            v[i] += (a[i]*timestep)*damping;
            p[i] += v[i]*timestep;
        }
    }
    return;
}

void mass::moveMass(const double dx, const double dy, const double dz) {
    p[0] += dx;
    p[1] += dy;
    p[2] += dz;
    return;
}

const void mass::printP() {
    std::cout << "P: " << p[0] << " " << p[1] << " " << p[2] << std::endl;
    return;
}

void mass::printV() {
    std::cout << "V: " << v[0] << " " << v[1] << " " << v[2] << std::endl;
    return;
}

void mass::printA() {
    std::cout << "A: " << a[0] << " " << a[1] << " " << a[2] << std::endl;
    return;
}

void mass::printSprings() {
    for (spring* item: s) {
        item->display();
    }
}

void mass::display() {
    std::cout <<"Mass Location: " << &(*this) << std::endl;
    std::cout << "Mass: " << m << std::endl;
    std::cout << "Fixed: " << fixed << std::endl;
    printP();
    printV();
    printA();
    printSprings();
    std::cout << std::endl;
    return;
}
/*
void mass::linkSpring(spring* aSpring) {
    for (spring* item : s) {
        if (item == aSpring) {
            return;
        }
    }
    s.push_back(aSpring);
    return;
}
*/

force mass::calcForce(bool print) {
    force theReturn(this);
    theReturn.addSpringForce(print);
    theReturn.addGravity();
    theReturn.addResultantForce();
    theReturn.addFrictionForce();
    return theReturn;
}

void drawSphere(double r, int lats, int longs) {
    int i, j;
    for(i = 0; i <= lats; i++) {
        double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
        double z0  = sin(lat0);
        double zr0 =  cos(lat0);

        double lat1 = M_PI * (-0.5 + (double) i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for(j = 0; j <= longs; j++) {
            double lng = 2 * M_PI * (double) (j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);

            glNormal3f(x * zr0, y * zr0, z0);
            glVertex3f(r * x * zr0, r * y * zr0, r * z0);
            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(r * x * zr1, r * y * zr1, r * z1);
        }
        glEnd();
    }
}

void mass::draw() {
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, RED);
    glTranslated(p[0], p[1], p[2]);
    drawSphere(.02,10,10);
    glPopMatrix();
}
