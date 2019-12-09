//
//  robot.hpp
//  ECD-A4


#ifndef robot_hpp
#define robot_hpp

#include <stdio.h>
#include <vector>
#include "mass.hpp"
#include "globalVars.h"
#include <tuple>
#include <cmath>
#include <iostream>
#include <random>


class robot {
public:
    // default constructor should make a cube
    robot() {
        
      //  for (int i = 0; i < 8; i ++) { }
    /*
        masses[0] = mass(defaultWeight,0,0,0,false);
        masses[1] = mass(defaultWeight,0,0.1,0,false);
        masses[2] = mass(defaultWeight,0,0,0.1,false);
        masses[3] = mass(defaultWeight,0,0.1,0.1,false);
        masses[4] = mass(defaultWeight,0.1,0,0,false);
        masses[5] = mass(defaultWeight,0.1,0.1,0,false);
        masses[6] = mass(defaultWeight,0.1,0,0.1,false);
        masses[7] = mass(defaultWeight,0.1,0.1,0.1,false);
      */
        double z = 0;
        double a = 0.1;
        masses.push_back(mass(defaultWeight,z,z,z,false));
        masses.push_back(mass(defaultWeight,z,a,z,false));
        masses.push_back(mass(defaultWeight,z,z,a,false));
        masses.push_back(mass(defaultWeight,z,a,a,false));
        masses.push_back(mass(defaultWeight,a,z,z,false));
        masses.push_back(mass(defaultWeight,a,a,z,false));
        masses.push_back(mass(defaultWeight,a,z,a,false));
        masses.push_back(mass(defaultWeight,a,a,a,false));
        
        for (int i = 0; i < 8; i++) {
            std::vector<std::tuple<bool,double,double,double,double>> row;
            for (int j = 0; j < 8; j++) {
                std::tuple<bool,double,double,double,double> aPair = std::make_tuple(false,0,0,0,0);
                row.push_back(aPair);
            }
            connections.push_back(row);
        }
        
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (i != j) {
                    double orinLength = massDistance(masses[i],masses[j]);
                    connections[i][j] = std::make_tuple(true,orinLength,defaultK,defaultAmplitde,defaultPhi);
                }
            }
        }
    
        robotTime = 0;
    };
    

    // copy constructor
    robot(const robot& rhs) {
        /*
        robotTime = rhs.robotTime;
        masses.clear();
        for (mass m : rhs.masses) {
            masses.push_back(m);
        }
        
        connections.resize(rhs.connections.size());
        for (int i = 0; i < connections.size(); i++) {
            connections[i].resize(rhs.connections[i].size());
        }
        
        for (int i = 0; i < rhs.connections.size(); i++) {
            for (int j = 0; j < rhs.connections[i].size(); j++) {
                connections[i][j] = rhs.connections[i][j];
            }
        }
         */
        *this = rhs;
    }
    // assignment operator
    robot& operator=(const robot& rhs) {
        if (this != &rhs) {
            robotTime = rhs.robotTime;
            masses.clear();
            for (mass m : rhs.masses) {
                masses.push_back(m);
            }
            
            connections.resize(rhs.connections.size());
            for (int i = 0; i < connections.size(); i++) {
                connections[i].resize(rhs.connections[i].size());
            }
            
            for (int i = 0; i < rhs.connections.size(); i++) {
                for (int j = 0; j < rhs.connections[i].size(); j++) {
                    connections[i][j] = rhs.connections[i][j];
                }
            }
        }
        return *this;
    }
    
    // destructor
    
    // spring force calculator
    void addSpringForce(mass& m, const int i) {
        for(int j = 0; j < connections.size(); j++) {
            bool springExists = std::get<0>(connections[i][j]);
            if (springExists) {
             // add the spring force
                double di[3];
                for (int k = 0; k < 3; k++) { di[k] = (m.p[k] - masses[j].p[k]); }
                double pyDist = massDistance(m, masses[j]);
                // stuff for the spring force
                double orinLen = std::get<1>(connections[i][j]);
                double b = std::get<3>(connections[i][j]);
                double c = std::get<4>(connections[i][j]);
                double pulseLen = orinLen + ((b*sin(w*robotTime+c))/8);
//                double pulseLen = orinLen ; // TARA TO MAKE THE BOUNCING ROBOT JUST UNCOMMENT THIS AND COMMENT THE ONE ABOVE
                double k = std::get<2>(connections[i][j]);
                double springForce = -k*(pyDist-pulseLen);
                // appply the force to the mass
                for(int ii = 0; ii < 3; ii++) {
                    m.f[ii] += springForce*di[ii]/pyDist;
                }
            }
        }
    };
    
    // simulate function
    void simulate(double dt = timestep, int steps = 1) {
        while (steps > 0) {
            // calculate the mass forces
            for (int i = 0; i < masses.size(); i++) {
                masses[i].resetForces();
                // get the 4 types of forces and save them within the mass.
                // DAN YOU CAN DO THIS AND JUST APPLY THE 4 FORCES TO THE F MEMBER OF THE MASS.
                // DO THEM IN 4 SEPERATE FUNCTIONS PLS.
                // WHEN YOU LOOP THROUHG THE MATRIX COPY THE NESTED FOR FROM THE CONSTRUCTOR
                // WHERE j = i AND  if (j != i)
                masses[i].addGravityForce();
                masses[i].addResultantForce();
                addSpringForce(masses[i],i);
                masses[i].addFrictionForce();
            }
            // update derivitives
            // loop thorugh masses
            for (mass& m : masses) {
                m.updateDerivitives();
            }
            robotTime += timestep;
            steps--;
        }
    }
    
    // add mass
    // takes a mass object to insert
    // IT DOES NOT CREATE ANY CONNECTIONS FOR THE MASS
    void addMass(mass& m, std::vector<int> connectMass) {
        masses.push_back(m);
        std::vector<std::tuple<bool,double,double,double,double>> temp;
        int j = connections.size();
        std::tuple<bool,double,double,double,double> nullPair = std::make_tuple(false,0,0,0,0);
        for(int i = 0; i < connections.size(); i++) {
            temp.push_back(nullPair);
            connections[i].push_back(nullPair);
        }
        temp.push_back(nullPair);
        connections.push_back(temp);
        for (int i : connectMass) {
            double len = massDistance(m,masses[i]);
            std::tuple<bool,double,double,double,double> aPair = std::make_tuple(true,len,defaultK,defaultAmplitde,defaultPhi);
            connections[i][j] = aPair;
            connections[j][i] = aPair;
        }
    }
    
    // remove mass
    // this must remove the row and column of the mass
    // this requires a lot of overhead so dont do it often
    // takes in the INDEX of the mass
    void removeMass(int m) {
        std::vector<std::vector<std::tuple<bool,double,double,double,double>>>::iterator row = connections.begin();
        row += m;
        connections.erase(row);
        for(int i = 0; i < connections.size(); i++) {
            std::vector<std::tuple<bool,double,double,double,double>>::iterator col = connections[i].begin();
            col += m;
            connections[i].erase(col);
        }
        // remove from the mass vector
        std::vector<mass>::iterator last = masses.begin();
        last += m;
        masses.erase(last);
        return;
    }
    void resetV() {
        for (mass m : masses) {
        for (int i = 0; i < 3; i++) {
            m.v[i] = 0;
            m.a[i] = 0;
            m.f[i] = 0;
        }}
    }
    // resize weighted graph
    void resetXYZ() {
        std::vector<double> end_CoM = centerOfMass();
        
        for (mass m : masses) {
            for (int i = 0 ; i < 3; i++) {
                m.p[i] -= end_CoM[i];
            }
        }
        double deepestUnder = 0;
        for (mass m : masses) {
            if (m.p[2] < 0 && m.p[2] < deepestUnder) {
                deepestUnder = m.p[2];
            }
        }
        
        if (deepestUnder < 0) {
            for (mass m : masses) {
                m.p[2] -= deepestUnder;
            }
        }
        return;
    }
    // add spring
    // this must be done between masses that already exist
    // takes in the index of the two masses and the spring components
    // it DOES NOT check if the masses already have a spring
    void addSpring(int m1, int m2, double k = defaultK, double amp = defaultAmplitde, double phi = defaultPhi) {
        double orinLen = massDistance(masses[m1], masses[m2]);
        std::tuple<bool,double,double,double,double> temp = std::make_tuple(true,orinLen,k,amp,phi);
        connections[m1][m2] = temp;
        connections[m2][m1] = temp;
        return;
    }
    // remove a spring
    // takes in the index of m1 and m2 in the vector, and removes the spring from the connection graph
    // must effect both sided of the matrix
    void removeSpring(int m1, int m2) {
        std::tuple<bool,double,double,double,double> temp = std::make_tuple(false,0,0,0,0);
        connections[m1][m2] = temp;
        connections[m2][m1] = temp;
        return;
    }
    
    // will reset the spring to defaults if no values given
    void alterSpring(int m1, int m2, double k = defaultK, double amp = defaultAmplitde, double phi = defaultPhi) {
        double len = std::get<1>(connections[m1][m2]);
        std::tuple<bool,double,double,double,double> temp = std::make_tuple(true,len,k,amp,phi);
        connections[m1][m2] = temp;
        connections[m2][m1] = temp;
        return;
    }
    // save function
    
    // distance between two masses
    const double massDistance(const mass& a, const mass& b)  {
        double theReturn = 0;
        for(int i = 0; i < 3; i++) { theReturn += pow((a.p[i]-b.p[i]),2.0); }
        return sqrt(theReturn);
    }

    // print connections
    void displayConnectionMatrix() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                std::cout << std::get<0>(connections[i][j]) << " ";
            }
            std::cout << std::endl;
        }
        return;
    }
    
    // robot draw spring functions
    // just write this to draw a single spring between two masses
    void drawSpring(const mass& m1, const mass& m2) {
        glLineWidth(5);
        glBegin(GL_LINES);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, RED);
        glVertex3d(m1.p[0],m1.p[1],m1.p[2]);
        glVertex3d(m2.p[0],m2.p[1],m2.p[2]);
        glEnd();

    };
    
    void drawAllSprings() {
        for (int i = 0; i < connections.size(); i++) {
            for (int j = i; j < connections.size(); j++) {
                if (j != i) {
                    if(std::get<0>(connections[i][j])) { drawSpring(masses[i],masses[j]); }
                }
            }
        }
    };
    
    void drawMasses() {
        for (mass& m : masses) { m.draw(); }
        return;
    }
    
    void drawRobot() {
        drawAllSprings();
        drawMasses();
        return;
    }
    // members
    double robotTime;
    // all the connected masses
    std::vector<mass> masses;
    // weighted graph for spring connections
    // double is L0,k,amplitude,phase
    std::vector<std::vector<std::tuple<bool,double,double,double,double>>> connections;
    
    std::vector<double> centerOfMass() {
        std::vector<double> topHalf = {0,0,0};
        double bottomHalf = 0;
        std::vector<double> theReturn = {0,0,0};
        for (mass& item : masses) {
            topHalf[0] += item.weight*item.p[0];
            topHalf[1] += item.weight*item.p[1];
            topHalf[2] += item.weight*item.p[2];
            bottomHalf += item.weight;
        }
        for (int i = 0; i < 3; i++) { theReturn[i] = topHalf[i]/bottomHalf; }
        return theReturn;
    }
    
};

#endif /* robot_hpp */
