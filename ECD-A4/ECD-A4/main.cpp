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
#include "taraRNG.hpp"
#include <algorithm>
#include <numeric>
#include <cmath>

const double timestep = 0.00001;
const double defaultWeight = 1.3;
const double maxRobotMass = 1;
const double defaultPhi = 0;
const double defaultAmplitde = .2;
const double defaultK = 7000;
const double friction_mu_k=0.8;// friction coefficient rubber-concrete
const double g[3] = {0,-50.81,0};
const double kc = 1000000;
const double w = .5;
const double damping = 0.8;

int width = 700;
int height = 700;
double t = 0;

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

Checkerboard checkerboard(3,3);
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
    glScalef(.71f, .71f, .71f);

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

double random_number(double start = 0 , double end = 1)
{
    static std::random_device e;
    static std::uniform_real_distribution<> dis(start, end);
    return dis(e);
}

int main(int argc, const char * argv[]) {
    
    int populationSize = 4;
    int parentSize = populationSize/2;
    
    double k_increment = 100;
    double amplitude_increment = 0.01;
    double phi_increment = 0.001 ;
    
    double robotSimulationTime = 5 / timestep;
    bool simulate = true;
    bool visualize = false;
    int evolutionIterations = 1;
    
    double movementResults[parentSize][evolutionIterations];
    
    vector<robot> ParentBots(parentSize);
    vector<robot> ChildrenBots(parentSize);
        
    // Make some random population!!
    cout << "Making Starting Parents" << endl;
    
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

     // DO VIZ STUFF
     // for framerate limiting
     double time = glfwGetTime();
     double lastTime = time;
     double deltaTime = time - lastTime;
     glfwInit();
     GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
     window = glfwCreateWindow( width, height, "ROBITS", NULL, NULL);
     /* Initialize the library*/
     if (!glfwInit())
         return -1;

    /* Create a windowed mode window and its OpenGL context*/
    if (!window) {
        glfwTerminate();
        return -1;
     }
     /* Make the window's context current */
    glfwMakeContextCurrent(window);
     
     // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    init_gl();
    
    for (int j = 0; j < evolutionIterations; j++) {

        // start by generating children from parents

        vector<robot> ChildrenBots(ParentBots.size());

        // loop through and MUTATE each parent's spring values into a single child
        cout << "Mutating..." << endl;
        for(int i = 0; i < ParentBots.size(); i++) {
            robot tempBuildingBot(ParentBots[i]);
            for (int ii = 0;  ii < tempBuildingBot.connections.size(); ii++) {
                for (int jj = ii; jj < tempBuildingBot.connections.size(); jj++) {
                    if (jj != ii) {
                        if (tempBuildingBot.masses.size() > 8) {
                            double kill_mass = random_number(0,1);
                            if (kill_mass > 0.5){
                                int mass_to_kill = floor(random_number(0, tempBuildingBot.masses.size()));
                                tempBuildingBot.removeMass(mass_to_kill);
                            }
                        }

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
                            tempBuildingBot.alterSpring(ii, jj, mutated_stiffness, mutated_amplitude, mutated_phi/1000);
                        }
                    }
                }
            }
            ChildrenBots[i] = tempBuildingBot;
        }
//        add masses randomly
        double random_mass;
        cout << "Adding Masses..." << endl;
        for (int kk = 0; kk < ChildrenBots.size(); kk++){
            random_mass = floor((rand_nummy()));
            for (int jj = 0; jj < random_mass; jj++){
                mass newMass(defaultWeight,rand_pos(),rand_pos(),rand_pos(),false);
                vector<pair<double,int>> distances;
                int count = 0;
                for(mass m: ChildrenBots[kk].masses){
                    distances.push_back(std::make_pair(ChildrenBots[kk].massDistance(newMass, m),count));
                    count++;
                }
                sort(distances.begin(), distances.end());
                vector<int> connectMass;
                for (int i =0; i<3;i++) { connectMass.push_back(distances[i].second); }
                ChildrenBots[kk].addMass(newMass,connectMass);
            }
        }

        cout << "Crossing Over..." << endl;
        for (int i = 0; i < ChildrenBots.size(); i+=2){
            int loopSize;
            if (ChildrenBots[i].masses.size() > ChildrenBots[i+1].masses.size()){
                loopSize = ChildrenBots[i+1].masses.size();
            }
            else {
                loopSize = ChildrenBots[i].masses.size();
            }
            for (int k=0; k< loopSize/2; k++) {

                std::tuple<bool, double, double, double, double> temp = ChildrenBots[i].connections[j][k];
                std::tuple<bool, double, double, double, double> tempB = ChildrenBots[i].connections[k][j];

                ChildrenBots[i].connections[j][k] = ChildrenBots[i+1].connections[j][k];
                ChildrenBots[i].connections[j][k] = ChildrenBots[i+1].connections[k][j];

                ChildrenBots[i+1].connections[j][k] = temp;
                ChildrenBots[i+1].connections[k][j] = tempB;

                mass tempM = ChildrenBots[i].masses[j];
                ChildrenBots[i].masses[j] = ChildrenBots[i+1].masses[j];
                ChildrenBots[i+1].masses[j] = tempM;

            }
        }
        // now we put all the children and parents together into one big array of vector
        cout << "Combining and Simulating..." << endl;
        vector<robot> Population(ParentBots);
        Population.insert(Population.end(), ChildrenBots.begin(), ChildrenBots.end());

        // Now we loop through the population and get the fitness
        vector<double> fitneese(Population.size());
        
        for (int i = 0; i < Population.size(); i++) {
            // for the given robot, get the current center of mass
            auto starting_CoM = Population[i].centerOfMass() ;
            // simulate the timeframe
            t = Population[i].robotTime;
            if (visualize == true){
            while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 && t < 5){
                Population[i].resetV();
//                Population[i].resetXYZ();
                   processInput(window);
                   /* Render here */
                   render(Population[i]);
                   /* Swap front and back buffers */
                   glfwSwapBuffers(window);
                   time = glfwGetTime();
                   lastTime = time;
                   deltaTime = time - lastTime;
                   
                 //  while (deltaTime <= frameTime) {
                   auto temp_com = Population[i].centerOfMass();
//                   cout << temp_com[0] << " " << temp_com[1] << " " << temp_com[2] << endl;;
                   cout << Population[i].robotTime << endl;
                   Population[i].simulate(timestep, 60);
                   
//                   vizRobot.simulate(timestep, 1);
//                   cout << vizRobot.robotTime << endl;
                   t = Population[i].robotTime;

                 
                   /* Poll for and process events */
                   glfwPollEvents();
                }
            } else {
                Population[i].resetV();
                auto temp_com = Population[i].centerOfMass();
                Population[i].simulate(timestep, robotSimulationTime);

                
                
            }
            
//            Population[i].simulate(timestep, robotSimulationTime);
            // now we calculate the ending center of mass
            auto ending_CoM = Population[i].centerOfMass();
            // Fitness is the magnitude of the change in center of mass
            double fitness = sqrt(pow(ending_CoM[0] - starting_CoM[0], 2.0) + pow(ending_CoM[1] - starting_CoM[1], 2.0) + pow( ending_CoM[2] - starting_CoM[2], 2.0));
            // Put the fitness into a vector of fitneese
            fitneese[i] = fitness;
        
        }

        // Once we have all the fitneese, we have to sort and get the best ones - rank based
        cout << "Ranking..." << endl;
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
    

    robot vizRobot = ParentBots[0];
    vizRobot.resetV();
    vizRobot.resetXYZ();
    
    if (simulate) {
       while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 && t < 10)
       {
           processInput(window);
           /* Render here */
           render(vizRobot);
           /* Swap front and back buffers */
           glfwSwapBuffers(window);
           time = glfwGetTime();
           lastTime = time;
           deltaTime = time - lastTime;
           
         //  while (deltaTime <= frameTime) {
           auto temp_com = vizRobot.centerOfMass();
           
           vizRobot.simulate(timestep, 1);
           cout << vizRobot.robotTime << endl;
           t = vizRobot.robotTime;

         
           /* Poll for and process events */
           glfwPollEvents();

       }
    }
    
}
