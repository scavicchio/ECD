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
#include "taraRNG.hpp"
#include "checkerboard.hpp"
#include "camera.hpp"
#include <algorithm>
#include <numeric>
#include <cmath>

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


//Checkerboard checkerboard(2,2);
//Camera camera;
//
//void init_gl() {
//    glEnable(GL_DEPTH_TEST);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
//    glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);
//    glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE);
//    glMaterialf(GL_FRONT, GL_SHININESS, 30);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//    checkerboard.create();
//}
//
//void render(robot& bot) {
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    checkerboard.draw();
//
//    glOrtho(-10.0f,10.0f,-10.0f,10.0f,-10.0f,10.0f);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    glRotatef(-25.0f,1.0f,0.0f,0.0f);
//    glRotatef(-45.0f,0.0f,1.0f,0.0f);
//    glRotatef(camera.getY(),0.0f,1.0f,0.0f);
//    glRotatef(camera.getZ(),0.0f,0.0f,1.0f);
//
//    bot.drawRobot();
//
//    glFlush();
//
//};
//
//void processInput(GLFWwindow *window) {
//    if (glfwGetKey(window,GLFW_KEY_W)) { camera.moveUp(); }
//    else if (glfwGetKey(window,GLFW_KEY_A)) { camera.moveLeft(); }
//    else if (glfwGetKey(window,GLFW_KEY_S)) { camera.moveDown(); }
//    else if (glfwGetKey(window,GLFW_KEY_D)) { camera.moveRight(); }
//    return;
//}

double random_number(int start = 0, int end = 1)
{
    static std::random_device e;
    static std::uniform_real_distribution<> dis(start, end);
    return dis(e);
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
    int parentSize = populationSize/2;
    
    double k_increment = 1000;
    double amplitude_increment = 0.1;
    double phi_increment = 0.01 ;
    
    double robotSimulationTime = 10 / timestep;
    
    int evolutionIterations = 2;
    
    vector<robot> ParentBots(parentSize);
    
    double movementResults[parentSize][evolutionIterations];

    // Make some random population!!
    for(int i = 0; i < ParentBots.size(); i++) {
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
        ParentBots[i] = tempBuildingBot;
        
    }
    for (int j = 0; j < evolutionIterations; j++) {
        
        // start by generating children from parents
        
        vector<robot> ChildrenBots(ParentBots.size());
        
        // loop through and MUTATE each parent's spring values into a single child
        
        for(int i = 0; i < ParentBots.size(); i++) {
            robot tempBuildingBot(ParentBots[i]);
            for (int ii = 0;  ii < tempBuildingBot.connections.size(); ii++) {
                for (int jj = ii; jj < tempBuildingBot.connections.size(); jj++) {
                    if (jj != ii) {
                        if (bool isConnected = get<0>(tempBuildingBot.connections[ii][jj])){
                            // This should alter springs with some random values
                            // but first we have to see if we are incrementing or decrementing
                            double starting_stiffness = get<2>(tempBuildingBot.connections[ii][jj]);
                            double starting_amplitude = get<3>(tempBuildingBot.connections[ii][jj]);
                            double starting_phi       = get<4>(tempBuildingBot.connections[ii][jj]);
                      
                            double randy_boi = random_number();
                            double mutated_stiffness ; double mutated_amplitude ; double mutated_phi ;
                            if (randy_boi >= 0.5) {
                                mutated_stiffness = starting_stiffness + k_increment;
                                mutated_amplitude = starting_amplitude + amplitude_increment;
                                mutated_phi       = starting_phi + phi_increment;
                            
                            } else {
                                mutated_stiffness = starting_stiffness - k_increment;
                                mutated_amplitude = starting_amplitude - amplitude_increment;
                                mutated_phi       = starting_phi - phi_increment;
                                
                            }
                            // now push these new random values to our bot
                            tempBuildingBot.alterSpring(ii, jj, mutated_stiffness, mutated_amplitude, mutated_phi);
                            
                        }
                    }
                }
            }
            ChildrenBots[i] = tempBuildingBot;
        }
        
        // now we put all the children and parents together into one big array of vector
        
        vector<robot> Population(ParentBots);
        Population.insert(Population.end(), ChildrenBots.begin(), ChildrenBots.end());
      
        // Now we loop through the population and get the fitness
        vector<double> fitneese(Population.size());
        for (int i = 0; i < Population.size(); i++) {
            // for the given robot, get the current center of mass
            auto starting_CoM = Population[i].centerOfMass() ;
            // simulate the timeframe
            Population[i].simulate(timestep, robotSimulationTime);
            // now we calculate the ending center of mass
            auto ending_CoM = Population[i].centerOfMass();
            // Fitness is the magnitude of the change in center of mass
            double fitness = sqrt(pow(2.0, ending_CoM[0] - starting_CoM[0]) + pow(2.0, ending_CoM[1] - starting_CoM[1]) + pow(2.0, ending_CoM[2] - starting_CoM[2]));
            // Put the fitness into a vector of fitneese
            fitneese[i] = fitness;
        }
        
        // Once we have all the fitneese, we have to sort and get the best ones - rank based
        auto idxs = tag_sort(fitneese);
        for (int i = 0; i < ParentBots.size(); i++) {
            int target_index = idxs[i];
            ParentBots[i] = Population[target_index];
            movementResults[i][j] = fitneese[target_index];
        }
        // Now we have a new set of ParentBots which are our top 10 robots
        // and can re-do the loop with the new ParentBots
    }
    for (int i = 0; i < evolutionIterations; ++i)
     {
         for (int j = 0; j < populationSize; ++j)
         {
             std::cout << movementResults[i][j] << ' ';
         }
         std::cout << std::endl;
     }
}
