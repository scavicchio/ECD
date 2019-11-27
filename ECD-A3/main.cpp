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

const double timestep = 0.00001;
const double f = 2; // frequemcy of length function in per/second
double w = 2*3.14*f/2;

const double damping = 0.98;
const double friction_mu_s=1;// friction coefficient rubber-concrete
const double friction_mu_k=0.8;// friction coefficient rubber-concrete
const double k_vertices_soft=2000;// spring constant of the edges
const double kc = 205000;
const double g[3] = {0,-9.81,0};
double k = 10000; //Nmss
const double defaultMassWeight = 100;

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

int main(int argc, char **argv) {
    // set global vars
    // initialize mass and array
    double weight = 100; //kg
    int fps = 60;
    double oneSecondOfSim = 1;
    oneSecondOfSim = 1/timestep;
    double simSteps = 1;
    simSteps = oneSecondOfSim/fps;
    double frameTime = 1;
    frameTime /= fps;
    //int steps = 500000;
    bool pulse = true;
    
    robot theRobot(pulse);
    
    bool debug = false;
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
    
    for (int i = 0; i < 10; i++) {
    /* Loop until the user closes the window */
       while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 && t < 3)
       {
           processInput(window);
           /* Render here */
           render(theRobot);
           /* Swap front and back buffers */
           glfwSwapBuffers(window);
           time = glfwGetTime();
           lastTime = time;
           deltaTime = time - lastTime;
           
           while (deltaTime <= frameTime) {
                theRobot.simulate(false,1,true);
                t += (timestep);
                if (debug) { break; }
                time = glfwGetTime();
                deltaTime = time - lastTime;
           }
         
           /* Poll for and process events */
           glfwPollEvents();
          // glfwGetWindowSize(window, &width, &height);
          // reshape(width, height);
           if (debug) {
               cout << "Current Sim Time: " << t << endl;
               vector<double> center = theRobot.centerOfMass();
               cout << "Center of Mass: " << center[0] << " " << center[1] << " " << center[2] << endl;
               cout << "===================" << endl;
           }
       }
        theRobot.reset();
        theRobot.randomizeSprings();
        t = 0;
    //
    }
    glfwTerminate();
    
    return 0;
}

