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

void robot::simulate(bool multicore, int simSteps, bool pulse) {
    std::vector<force> forces;
    
    while (simSteps > 0) {
         forces.clear();
         // get the forces
        // this can become parallel later
        for (std::vector<mass>::iterator item = masses.begin(); item != masses.end(); item++) {
            forces.push_back(force(&(*item),pulse,true));
        }
        int i = 0;
        // this can also become parallel later
        for (std::vector<mass>::iterator item = masses.begin(); item != masses.end(); item++) {
            item->updateDerivitives(forces[i],damping);
            i++;
        }
        robotTime +=timestep;
        simSteps--;
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
    robot* parent = this;
    mass m1(weight,0,0,0,parent);
    mass m2(weight,0,.1,0,parent);
    mass m3(weight,0,0,.1,parent);
    mass m4(weight,0,.1,.1,parent);
    mass m5(weight,.1,0,0,parent);
    mass m6(weight,.1,.1,0,parent);
    mass m7(weight,.1,0,.1,parent);
    mass m8(weight,.1,.1,.1,parent);
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
        item->b = generateRandom(0,1);
     //   item->c = generateRandom(0,6.28);
        item->k *= generateRandomPercentage();
      //  item->orinLen = item->pulseLength();
    }
}

void robot::reset() {
    robotTime = 0;
    
    masses[0].p[0] = 0; masses[0].p[1] = 0 ;masses[0].p[2] = 0;
    masses[1].p[0] = 0; masses[1].p[1] = .1 ;masses[1].p[2] = 0;
    masses[2].p[0] = 0; masses[2].p[1] = 0 ;masses[2].p[2] = .1;
    masses[3].p[0] = 0; masses[3].p[1] = .1 ;masses[3].p[2] = .1;
    masses[4].p[0] = .1; masses[4].p[1] = 0 ;masses[4].p[2] = 0;
    masses[5].p[0] = .1; masses[5].p[1] = .1 ;masses[5].p[2] = 0;
    masses[6].p[0] = .1; masses[6].p[1] = 0 ;masses[6].p[2] = .1;
    masses[7].p[0] = .1; masses[7].p[1] = .1 ;masses[7].p[2] = .1;
    
    for (std::vector<mass>::iterator item = masses.begin(); item != masses.end(); item++) {
        item->v[0] = 0; item->v[1] = 0; item->v[2] = 0;
        item->a[0] = 0; item->a[1] = 0; item->a[2] = 0;
    }
    
    //equalize();

    setOnGround();
/*
    for (mass& m : masses) {
        m.moveMass(0,1,0);
    }
    */
    return;
}

// THIS WILL MOVE MASSES TO EQUALIZE THE SPRING FORCES TO BE VERY SMALL
// threshold represents the maximum allowed spring force left in the object
// depth represents the recursion depth needed, max depth is 10.
void robot::equalize(double threshold, int maxDepth, int currentDepth) {
           // for a single mass (the first one) - will move the other masses so that they have no spring force left
           // then loop through the next ones (but ignoring the ones already hit
           // this is almost the same algo that links the masses and springs!
    if (maxDepth == currentDepth) {
        std::cout << "REACHED MAX EQUALIZER DEPTH. max spring force: " << calcMaxSpringForce() << std::endl;
    }
    else if (calcMaxSpringForce() < threshold) {
        std::cout << calcMaxSpringForce() << std::endl;
        std::cout << "threshold met at depth: " << currentDepth << " max spring force: " << calcMaxSpringForce() << std::endl;
    }
    else {/*
         for(int i = 0; i < masses.size(); i++) {
             for (spring* s : masses[i].s) {
                 force aForce(&masses[1],pulse,false);
                 aForce.getSingleSpringForce(s,pulse);
                 aForce.body->updateDerivitives(aForce);
             }
         }*/
        //equalize(threshold,maxDepth,--currentDepth);
     }
    return;
};

// this will move the robot up in case any of the equalizer put it underground
void robot::setOnGround() {
    double yOffset = 0;
    for(std::vector<mass>::iterator m = masses.begin(); m != masses.end(); m++) {
        if(m->p[1] < 0 && abs(m->p[1]) > yOffset ) {
            yOffset = abs(m->p[1]);
        }
    }
    for(std::vector<mass>::iterator m = masses.begin(); m != masses.end(); m++) {
        m->moveMass(0, yOffset, 0);
    }
    return;
}

double robot::calcMaxSpringForce() {
    double theReturn = 0;
    double aForce = 0;
    for(std::vector<spring>::iterator s = springs.begin(); s != springs.end(); s++) {
        aForce = abs(s->calcCurrentSpringForce(pulse));
        if (aForce > theReturn) { theReturn = aForce; }
    }
    return theReturn;
}

robot::robot(const robot& rhs) {
    masses = generateMasses();
    springs = generateSprings();
    linkMassSpring();
    /*
    masses.clear();
    springs.clear();

    for(int i = 0; i < rhs.masses.size(); i++) {
        mass temp = rhs.masses[i];
        temp.s.clear();
        masses.push_back(temp);
    }
    
    std::vector<int> m1_ids;
    std::vector<int> m2_ids;
    for (int j = 0; j < masses.size(); j++) {
     for (int k = j; k < masses.size(); k++) {
         if (j != k) {
         m1_ids.push_back(j);
         m2_ids.push_back(k);
         }
     }}
    for(int i = 0; i < rhs.springs.size(); i++) {
        spring temp = rhs.springs[i];
        temp.m1 = &masses[m1_ids[i]];
        temp.m2 = &masses[m2_ids[i]];
        springs.push_back(rhs.springs[i]);
        
        bool m1Exist = false;
        bool m2Exist = false;
        for (spring* sp : masses[m1_ids[i]].s) {
            if(sp == &springs[i]) { m1Exist = true; break; }
        }
        for (spring* sp : masses[m2_ids[i]].s) {
            if(sp == &springs[i]) { m2Exist = true; break; }
        }
        if(!m1Exist) { masses[m1_ids[i]].s.push_back(&springs[i]); }
        if(!m2Exist) { masses[m2_ids[i]].s.push_back(&springs[i]); }

    }
     */
    int i = 0;
    for (mass& m : masses) {
        m.m = rhs.masses[i].m;
        for (int j = 0; j < 3; j++) {
            m.p[j] = rhs.masses[i].p[j];
            m.v[i] = rhs.masses[i].v[j];
            m.a[i] = rhs.masses[i].a[j];
        }
        m.fixed = rhs.masses[i].fixed;
    }
    
    i = 0;
    for (spring& s : springs) {
        s.k = rhs.springs[i].k;
        s.c = rhs.springs[i].c;
        s.orinLen = rhs.springs[i].orinLen;
        s.b = rhs.springs[i].b;
    }
    
    robotTime = rhs.robotTime;
    pulse = rhs.pulse;
}


robot& robot::operator=(const robot& rhs) {
    if (this != &rhs)  {
        masses = generateMasses();
        springs = generateSprings();
        linkMassSpring();
        /*
        masses.clear();
        for(int i = 0; i < rhs.masses.size(); i++) {
            mass temp = rhs.masses[i];
            temp.s.clear();
            masses.push_back(temp);
        }
        springs.clear();
        std::vector<int> m1_ids;
        std::vector<int> m2_ids;
        for (int j = 0; j < masses.size(); j++) {
         for (int k = j; k < masses.size(); k++) {
             if (j != k) {
             m1_ids.push_back(j);
             m2_ids.push_back(k);
             }
         }}
        for(int i = 0; i < rhs.springs.size(); i++) {
            spring temp = rhs.springs[i];
            temp.m1 = &masses[m1_ids[i]];
            temp.m2 = &masses[m2_ids[i]];
            springs.push_back(rhs.springs[i]);
            
            bool m1Exist = false;
            bool m2Exist = false;
            for (spring* sp : masses[m1_ids[i]].s) {
               if(sp == &springs[i]) { m1Exist = true; break; }
            }
            for (spring* sp : masses[m2_ids[i]].s) {
               if(sp == &springs[i]) { m2Exist = true; break; }
            }
            if(!m1Exist) { masses[m1_ids[i]].s.push_back(&springs[i]); }
            if(!m2Exist) { masses[m2_ids[i]].s.push_back(&springs[i]); }
        }*/
        int i = 0;
        for (mass& m : masses) {
           m.m = rhs.masses[i].m;
           for (int j = 0; j < 3; j++) {
               m.p[j] = rhs.masses[i].p[j];
               m.v[i] = rhs.masses[i].v[j];
               m.a[i] = rhs.masses[i].a[j];
           }
           m.fixed = rhs.masses[i].fixed;
        }

        i = 0;
        for (spring& s : springs) {
           s.k = rhs.springs[i].k;
           s.c = rhs.springs[i].c;
           s.orinLen = rhs.springs[i].orinLen;
           s.b = rhs.springs[i].b;
        }
        
        robotTime = rhs.robotTime;
        pulse = rhs.pulse;
    }
    return *this;
}

void robot::mutateSprings() {
    for (std::vector<spring>::iterator item = springs.begin(); item != springs.end(); item++) {
        item->b += 0.01;
        item->k += 1000;
    }
}
