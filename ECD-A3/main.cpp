//
//  main.cpp
//  ECD-Project
//
//  Created by Salvy Cavicchio on 11/1/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#include "Header.h"
#include <GLUT/GLUT.h>
#include <GLFW/glfw3.h>
#include "globalVars.h"

using namespace std;

vector<mass> masses;
vector<spring> springs;
vector<force> forces;
double t = 0;
double c = 0;

const double f = 1; // frequemcy of length function in per/second
double w = 2*3.14*f/2;

const double damping = 0.9;
const double friction_mu_s=1;// friction coefficient rubber-concrete
const double friction_mu_k=0.8;// friction coefficient rubber-concrete
const double k_vertices_soft=200000;// spring constant of the edges
const double kc = 2000;
const double g[3] = {0,-9.81,0};
double k = 10000; //Nmss
const double defaultMassWeight = .1;

const double timestep = 0.00001;

int width = 700;
int height = 700;

Checkerboard checkerboard(10,10);
Camera camera;

// MORE FUNCTIONS
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

void render(robot& theRobot) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    checkerboard.draw();
    
    // camera transforms
    glOrtho(-10.0f,10.0f,-10.0f,10.0f,-10.0f,10.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-25.0f,1.0f,0.0f,0.0f);
    glRotatef(-45.0f,0.0f,1.0f,0.0f);
    glRotatef(camera.getY(),0.0f,1.0f,0.0f);
    glRotatef(camera.getZ(),0.0f,0.0f,1.0f);

    theRobot.draw();
    
    glFlush();
};

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window,GLFW_KEY_W)) { camera.moveUp(); }
    else if (glfwGetKey(window,GLFW_KEY_A)) { camera.moveLeft(); }
    else if (glfwGetKey(window,GLFW_KEY_S)) { camera.moveDown(); }
    else if (glfwGetKey(window,GLFW_KEY_D)) { camera.moveRight(); }
    return;
}

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

int main(int argc, char **argv) {
    // set global vars
    // initialize mass and array
    
    double weight = 0.1; //kg
    int fps = 30;

    double oneSecondOfSim = 1;
    oneSecondOfSim = 1/timestep;
    double simSteps = 1;
    simSteps = oneSecondOfSim/fps;
    double frameTime = 1;
    frameTime /= fps;
    //int steps = 500000;
    bool pulse = true;
    bool simulate = true;
    bool debug = false;
    
    
    robot theRobot(pulse);

    for (mass& m : theRobot.masses) {
        m.moveMass(0,0,0);
    }
   
    
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
    
    // for framerate limiting
    double time = glfwGetTime();
    double lastTime = time;
    double deltaTime = time - lastTime;
    
    int geneSize = 10;

    vector<robot> parent_bots(geneSize);
    vector<robot> children_bots(geneSize);
    
    // create the robot vector
    for(int i = 0; i < geneSize; i++) {
        theRobot.randomizeSprings();
        theRobot.reset();
        parent_bots[i] = theRobot;
    }

    // how many iterations to evolve
    int evoloutionIterations = 2;
    // how much time for each simulation
    double simTimeLength = 1;
    
    
    // the results arrays
    double movementResults[geneSize][evoloutionIterations];
    vector<double> allBots_results(2*geneSize);
    
    
    // for all the evoloution iterations
    for (int j = 0; j < evoloutionIterations; j++) {
        // we don't have to loop over all the parents to begin with, let's just get parents and children then loop
        // for each robot in the vector
//        for (int i = 0; i < parent_bots.size(); i++) {
//            // simulate the timeframe
//            while(parent_bots[i].robotTime < simTimeLength) { parent_bots[i].simulate(); }
//            // find the current value
//            cout << parent_bots[i].centerOfMass()[0] << endl;
//            movementResults[i][j] = parent_bots[i].centerOfMass()[0];
//            cout << movementResults[i][j] << endl;
//        }
//
        
        // now do whatever evoloution stuff
        // for each parent robot, let's duplicate it into a new vector of robots called "children_bots"
//        // create the starting children randomly too
//        for (int i = 0; i < geneSize; i++) {
//            theRobot.randomizeSprings();
//            theRobot.reset();
//            children_bots[i] = theRobot;
//        }
        
//        children_bots.clear();
        for (int i = 0; i < children_bots.size(); i++) {
            robot temp_bot(parent_bots[i]);
            // make a small change to the springs and reset the cubes
            temp_bot.mutateSprings();
            temp_bot.reset();
            children_bots[i] = temp_bot;
        }

        //now let's loop through and do 1/2 crossover on the second half by setting child 1 spring = child 2 spring
        for (int i = 0; (i*2) < children_bots.size(); i++) {
            
            robot tmp_crossed_child1 = children_bots[2*i];
            robot tmp_crossed_child2 = children_bots[2*i+1];
            
            // only switch the second half of springs!!
            for (int kk = 13; kk < tmp_crossed_child1.springs.size(); kk ++ ) {
                tmp_crossed_child1.springs[k].b = children_bots[2*i+1].springs[kk].b;
                tmp_crossed_child2.springs[k].b = children_bots[2*i].springs[kk].b;
                
                tmp_crossed_child1.springs[k].k = children_bots[2*i+1].springs[kk].k;
                tmp_crossed_child2.springs[k].k = children_bots[2*i].springs[kk].k;
            }

            children_bots[2*i] = tmp_crossed_child1;
            children_bots[2*i+1] = tmp_crossed_child2;
          }
        
        
        cout << "starting allbots" << endl;
        
        //now combine parents and children bots into all bots
        
        vector<robot> allBots(parent_bots);
        allBots.insert(allBots.end(), children_bots.begin(), children_bots.end());
        
        for (int i = 0; i < allBots.size(); i++) {
            // simulate the timeframe
            while(allBots[i].robotTime < simTimeLength) { allBots[i].simulate(false,1,true); }
            // find the current value
            cout << "New Center of Mass: " << allBots[i].centerOfMass()[0] << endl;
            allBots_results[i] = abs(allBots[i].centerOfMass()[0]);
        }
        
        // get the indexes of for the sorted elements
        auto idxs = tag_sort(allBots_results);
        for (auto && elem : idxs) { std::cout << elem << " : " << allBots_results[elem] << std::endl; }
        
        // wipe away the current parents and replace with the top half of allBots
        for (int i = 0; i < parent_bots.size(); i++) {
            int target_index = idxs[i];
            parent_bots[i] = allBots[target_index];
            movementResults[i][j] = allBots_results[target_index];
            
        }
        cout << " " << endl;
    }


    
    
    
    /* Loop until the user closes the window */
    if (simulate) {
       while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 && t < 10)
       {
           processInput(window);
           /* Render here */
           render(parent_bots[0]);
           /* Swap front and back buffers */
           glfwSwapBuffers(window);
           time = glfwGetTime();
           lastTime = time;
           deltaTime = time - lastTime;
           
         //  while (deltaTime <= frameTime) {
           parent_bots[0].simulate(false,simSteps,true);
           t = parent_bots[0].robotTime;
        //        if (!debug) { break; }
       //         time = glfwGetTime();
        //        deltaTime = time - lastTime;
      //     }
         
           /* Poll for and process events */
           glfwPollEvents();
          // glfwGetWindowSize(window, &width, &height);
          // reshape(width, height);
           if (debug) {
               cout << "Current Sim Time: " << t << endl;
               vector<double> center = parent_bots[9].centerOfMass();
               cout << "Center of Mass: " << center[0] << " " << center[1] << " " << center[2] << endl;
               cout << "===================" << endl;
           }
       }
    }
    else {
        cout << "Not Visualizing..." << endl;
    }
    //
    
    glfwTerminate();
    
    
    // loop through movementResults

    for (int i = 0; i < evoloutionIterations; ++i)
     {
         for (int j = 0; j < geneSize; ++j)
         {
             std::cout << movementResults[i][j] << ' ';
         }
         std::cout << std::endl;
     }

    return 0;
}
