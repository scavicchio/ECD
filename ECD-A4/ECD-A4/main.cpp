//
//  main.cpp
//  ECD-A4
//
//  Created by Salvy Cavicchio on 12/5/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#include <iostream>
#include <GLUT/GLUT.h>
#include "GLFW/glfw3.h"
#include "globalVars.h"
#include "robot.hpp"
#include "checkerboard.hpp"
#include "camera.hpp"
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


Checkerboard checkerboard(2,2);
Camera camera;

void init_gl() {
    glEnable(GL_DEPTH_TEST);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
    glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE);
    glMaterialf(GL_FRONT, GL_SHININESS, 30);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    checkerboard.create();
}

void render(robot& bot) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    checkerboard.draw();
    
    glOrtho(-10.0f,10.0f,-10.0f,10.0f,-10.0f,10.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-25.0f,1.0f,0.0f,0.0f);
    glRotatef(-45.0f,0.0f,1.0f,0.0f);
    glRotatef(camera.getY(),0.0f,1.0f,0.0f);
    glRotatef(camera.getZ(),0.0f,0.0f,1.0f);
    
    bot.drawRobot();
        
    glFlush();
    
};

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window,GLFW_KEY_W)) { camera.moveUp(); }
    else if (glfwGetKey(window,GLFW_KEY_A)) { camera.moveLeft(); }
    else if (glfwGetKey(window,GLFW_KEY_S)) { camera.moveDown(); }
    else if (glfwGetKey(window,GLFW_KEY_D)) { camera.moveRight(); }
    return;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    int fps = 60;
    double oneSecondOfSim = 1;
    oneSecondOfSim = 1/timestep;
    double simSteps = 1;
    simSteps = oneSecondOfSim/fps;
    double frameTime = 1;
    frameTime /= fps;
    
    robot aRobot;
    aRobot.displayConnectionMatrix();
    cout << endl;
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
    
    int populationSize = 20;
    int evolutionIterations = 2;
    int parentSize = populationSize/2;
    double k_increment = 1000;
    double amplitude_increment = 0.1;
    double phi_increment = 0.01 ;
    
    vector<robot> ParentBots(parentSize);
    vector<robot> ChildrenBots(parentSize);
    vector<robot> Population(populationSize);
        
    // Make some random population!!
    for(int i = 0; i < Population.size(); i++) {
        robot tempBuildingBot;
        for (int ii = 0;  ii < tempBuildingBot.connections.size(); ii++) {
            for (int jj = ii; jj < tempBuildingBot.connections.size(); jj++) {
                if (jj != ii) {
                    if (bool isConnected = get<0>(tempBuildingBot.connections[ii][jj])){
                        // This should alter springs with some random values
                        tempBuildingBot.alterSpring(ii, jj, rand_K(), rand_B(), rand_C());
                    }
                }
            }
        }
        Population[i] = tempBuildingBot;
    }

    
    for (int j = 0; j < evolutionIterations; j++) {
        

        
        
        
        
    }
    
    
    
    
}
    
    
    return 0;
}
