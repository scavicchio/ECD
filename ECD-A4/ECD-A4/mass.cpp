//
//  mass.cpp
//  ECD-A4
//
//  Created by Salvy Cavicchio on 12/5/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#include "mass.hpp"

//void drawSphere(double r, int lats, int longs) {
//    int i, j;
//    for(i = 0; i <= lats; i++) {
//        double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
//        double z0  = sin(lat0);
//        double zr0 =  cos(lat0);
//
//        double lat1 = M_PI * (-0.5 + (double) i / lats);
//        double z1 = sin(lat1);
//        double zr1 = cos(lat1);
//
//        glBegin(GL_QUAD_STRIP);
//        for(j = 0; j <= longs; j++) {
//            double lng = 2 * M_PI * (double) (j - 1) / longs;
//            double x = cos(lng);
//            double y = sin(lng);
//
//            glNormal3f(x * zr0, y * zr0, z0);
//            glVertex3f(r * x * zr0, r * y * zr0, r * z0);
//            glNormal3f(x * zr1, y * zr1, z1);
//            glVertex3f(r * x * zr1, r * y * zr1, r * z1);
//        }
//        glEnd();
//    }
//}
//
//void mass::draw() {
//    glPushMatrix();
//    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, RED);
//    glTranslated(p[0], p[1], p[2]);
//    drawSphere(.02,10,10);
//    glPopMatrix();
//}
//void spring::draw() {
//    glLineWidth(5);
//    glBegin(GL_LINES);
//    glVertex3d(m1->p[0],m1->p[1],m1->p[2]);
//    glVertex3d(m2->p[0],m2->p[1],m2->p[2]);
//    glEnd();
//}
