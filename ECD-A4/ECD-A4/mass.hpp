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
#include <GLUT/GLUT.h>
#include <cmath>

struct mass {
    mass(double m, double x, double y, double z, bool fix) : weight(m), fixed(fix) {
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
        for (int i = 0; i < 3; i++) {
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
            for (int i = 0; i < 3; i++) {
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
    

    void addGravityForce() {
        for (int i = 0; i < 3; i++) {
            f[i] += weight*g[i];
        }
        return;
    }

    void addFrictionForce() {
        // will only add friction force if the body is on (or below) the ground AND the normal force is < 0
        if (p[1] <= 0) {
            // a friction component is calculated as f_i = fN*u
            double fN = f[1]*friction_mu_k;
            if (f[0] < fN) { // then zero the velocity (ie the friction is slowing the mass)
                v[0] = 0;
            }
            else if (f[0] > 0) { f[0] -= fN; }
            else { f[0] += fN; }
            
            if (f[2] < fN) { // then zero the velocity (ie the friction is slowing the mass)
                v[2] = 0;
            }
            else if (f[2] > 0) { f[2] -= fN; }
            else { f[2] += fN; }
        }
        return;
    }

    void addResultantForce() {
        if (p[1] <= 0) {
            // for bouncing on the ground
      //      body->p[1] = 0;
        //    body->v[1] = 0;
          //  body->a[1] = 0;
            f[1] -= kc*(p[1])*0.5;
//            f[1] = -f[1];
        }
        return;
    }

    void updateDerivitives(const double d = damping) {
        if (!fixed) {
            for (int i = 0; i < 3; i++) {
                a[i] = f[i]/weight*d;
                v[i] += (a[i]*timestep)*d;
                p[i] += v[i]*timestep*d;
            }
        }
        return;
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

    void draw() {
        glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, RED);
        glTranslated(p[0], p[1], p[2]);
        drawSphere(.02,10,10);
        glPopMatrix();
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
