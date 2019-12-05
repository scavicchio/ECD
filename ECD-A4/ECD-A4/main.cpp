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

const double timestep = 0.0001;
const double defaultWeight = 0.1;
const double maxRobotMass = 1;
const double defaultPhi = 0;
const double defaultAmplitde = 1;
const double defaultK = 20000;

using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    robot aRobot;
    aRobot.displayConnectionMatrix();
    cout << endl;
    aRobot.connections[1][6] = std::make_tuple(false,0,0,0,0);
    aRobot.displayConnectionMatrix();

    cout << "COPY TEST" << endl;
    robot bRobot(aRobot);
    bRobot.displayConnectionMatrix();
    cout << "ASSIGNMENT TEST" << endl;
    robot cRobot;
    cRobot = bRobot;
    cRobot.displayConnectionMatrix();
    return 0;
}
