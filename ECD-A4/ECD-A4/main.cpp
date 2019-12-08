//
//  main.cpp
//  ECD-A4
//
//  Created by Salvy Cavicchio on 12/5/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#include <iostream>
#include "globalVars.h"
#include "robot.hpp"
#include <algorithm>
#include <numeric>

const double timestep = 0.0001;
const double defaultWeight = 0.1;
const double maxRobotMass = 1;
const double defaultPhi = 0;
const double defaultAmplitde = 1;
const double defaultK = 20000;
const double friction_mu_k=0.8;// friction coefficient rubber-concrete
const double g[3] = {0,-9.81,0};
const double kc = 2000;
const double w = 1;
const double damping = 0.9;

using namespace std;


template<typename Type>
vector<size_t> tag_sort(const vector<Type>& vec)
{
    vector<size_t> result(vec.size());
    iota(begin(result), end(result), 0);
    sort(begin(result), end(result),
            [&vec](const auto & lhs, const auto & rhs)
            {
                return vec[lhs] > vec[rhs];
            }
    );
    return result;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    
    robot aRobot;
    aRobot.connections[1][6] = std::make_tuple(false,0,0,0,0);
    aRobot.connections[6][1] = std::make_tuple(false,0,0,0,0);

    aRobot.displayConnectionMatrix();

    cout << "COPY TEST" << endl;
    robot bRobot(aRobot);
    bRobot.displayConnectionMatrix();
    cout << "ASSIGNMENT TEST" << endl;
    robot cRobot;
    cRobot = bRobot;
    cRobot.displayConnectionMatrix();
    cout << endl << endl;
    
    int populationSize = 20;
    int evolutionIterations = 2;
    int parentSize = populationSize/2;
    
    
    vector<robot> startingParentBots(parentSize);
    vector<robot> startingChildrenBots(parentSize);
    robot buildingBot;
    
    for(int i = 0; i < startingParentBots.size(); i++) {
        startingParentBots[i] = buildingBot;
    }
    
    for (int j = 0; j < evoloutionIterations; j++) {
        
        // So we start by generating some kids
        for (int i = 0; i < children_bots.size(); i++) {
            robot temp_bot(parent_bots[i]);
            // make a small change to the springs and reset the cubes
            
            children_bots[i] = temp_bot;
        }

    }
    

    
    
    
    
    
    return 0;
}
