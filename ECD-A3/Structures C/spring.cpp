//
//  spring.cpp
//  ECD-A3
//
//  Created by Salvy Cavicchio on 11/7/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#include "spring.hpp"
#include <cmath>
#include "mass.hpp"
#include <iostream>
#include <GLUT/GLUT.h>
#include "globalVars.h"

/*Each spring object has the following properties: k(spring constraint, [N/m]), L0(original rest length, [meters]), and the indices of the two masses it connects: m1, m2
 */

const void spring::display() {
    std::cout << "Spring: k=" << k << " m1: " << m1 << " m2: " << m2 << std::endl;
    return;
}

const double spring::length() const {
    double theReturn = 0;
    for(int i = 0; i < 3; i++) { theReturn += pow((m1->p[i]-m2->p[i]),2); }
    return sqrt(theReturn);
}

// will return negative if compression.
double spring::calcCurrentSpringForce(const bool pulse) {
    if(pulse) {
       // std::cout << "Pulsing: " << pulseLength() << std::endl;
        return k*(length()-pulseLength());
    }
    return k*(length()-orinLen);
}
    
const double spring::pulseLength() {
    double theReturn = orinLen+abs((b*sin(w*t+c)))/2;
  //  std::cout << theReturn << std::endl;
    return theReturn;
}

spring& spring::operator=(const spring& right) {
    if (this != &right) {
        k = right.k;
        orinLen = right.orinLen;
        m1 = right.m1;
        m2 = right.m2;
        b = right.b;
        c = right.c;
    }
    return *this;
}

spring::spring(const spring& rhs) {
    k = rhs.k;
    orinLen = rhs.orinLen;
    m1 = rhs.m1;
    m2 = rhs.m2;
    b = rhs.b;
    c = rhs.c;
}

std::ostream& operator<<(std::ostream& os, const spring& item) {
    os << "Spring&: " << &item << "\n" << "Current Length: " << item.length() << std::endl;
    return os;
}

bool operator==(const spring& lhs, const spring& rhs) {
    if (rhs.m1 == lhs.m1 && rhs.m2 == lhs.m2) { return true; }
    return false;
}

void spring::draw() {
    glLineWidth(5);
    glBegin(GL_LINES);
    glVertex3d(m1->p[0],m1->p[1],m1->p[2]);
    glVertex3d(m2->p[0],m2->p[1],m2->p[2]);
    glEnd();
}

