//
//  robot.hpp
//  ECD-A4
//
//  Created by Salvy Cavicchio on 12/5/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#ifndef robot_hpp
#define robot_hpp

#include <stdio.h>
#include <vector>
#include "mass.hpp"
#include "globalVars.h"
#include <tuple>
#include <cmath>
#include <iostream>


class robot {
public:
    // default constructor should make a cube
    robot() {
        
        for (int i = 0; i < 8; i ++) { masses.push_back(mass(defaultWeight,0,0,0,false));}
        
        masses[0] = mass(defaultWeight,0,0,0,false);
        masses[1] = mass(defaultWeight,0,0.1,0,false);
        masses[2] = mass(defaultWeight,0,0,0.1,false);
        masses[3] = mass(defaultWeight,0,0.1,0.1,false);
        masses[4] = mass(defaultWeight,0.1,0,0,false);
        masses[5] = mass(defaultWeight,0.1,0.1,0,false);
        masses[6] = mass(defaultWeight,0.1,0,0.1,false);
        masses[7] = mass(defaultWeight,0.1,0.1,0.1,false);
        
        for (int i = 0; i < 8; i++) {
            std::vector<std::tuple<bool,double,double,double,double>> row;
            for (int j = 0; j < 8; j++) {
                std::tuple<bool,double,double,double,double> aPair = std::make_tuple(false,0,0,0,0);
                row.push_back(aPair);
            }
            connections.push_back(row);
        }
        
        for (int i = 0; i < 8; i++) {
            for (int j = i; j < 8; j++) {
                if (i != j) {
                    double orinLength = massDistance(masses[i],masses[j]);
                    connections[i][j] = std::make_tuple(true,orinLength,defaultK,defaultAmplitde,defaultPhi);
                }
            }
        }
    
        robotTime = 0;
    };
    
    const double massDistance(const mass& a, const mass& b)  {
        double theReturn = 0;
        for(int i = 0; i < 3; i++) { theReturn += pow((a.p[i]-b.p[i]),2); }
        return sqrt(theReturn);
    }

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
    
    // simulate function
    
    // add mass
    
    // resize weighted graph
    
    // add springs
    
    // save function
    
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
    
    // members
    double robotTime;
    // all the connected masses
    std::vector<mass> masses;
    // weighted graph for spring connections
    // double is L0,k,amplitude,phase
    std::vector<std::vector<std::tuple<bool,double,double,double,double>>> connections;
};

#endif /* robot_hpp */
