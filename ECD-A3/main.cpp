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

const double timestep = 0.0001;
const double f = 1;
double w = 2*3.14*f/2;

const double damping = 0.7;
const double friction_mu_s=1;// friction coefficient rubber-concrete
const double friction_mu_k=0.8;// friction coefficient rubber-concrete
const double k_vertices_soft=2000;// spring constant of the edges
const double kc = 200000;
const double g[3] = {0,-9.81,0};

int width = 700;
int height = 700;

void simulate(bool multicore = false, int maxSteps = 1, bool pulse = false) {
    
    while (maxSteps > 0) {
         forces.clear();
         // get the forces
        // this can become parallel later
        for (vector<mass>::iterator item = masses.begin(); item != masses.end(); item++) {
            forces.push_back(force(&(*item),pulse));
        }
        int i = 0;
        // this can also become parallel later
        for (vector<mass>::iterator item = masses.begin(); item != masses.end(); item++) {
            item->updateDerivitives(forces[i]);
            i++;
        }
        t = t+=timestep;
        maxSteps--;
    }
    return;
}

// Checkerboard Class
class Checkerboard {
  int displayListId;
  int width;
  int depth;
public:
  Checkerboard(int width, int depth): width(width), depth(depth) {}
  double centerx() {return width / 2;}
  double centerz() {return depth / 2;}
  void create() {
    displayListId = glGenLists(1);
    glNewList(displayListId, GL_COMPILE) ;
    GLfloat lightPosition[] = {4, 3, 7, 1} ;
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition) ;
    glBegin(GL_QUADS) ;
    glNormal3d(0, 1, 0) ;
    for (int x = 0; x < width - 0.5; x++) {
      for (int z = 0; z < depth - 0.5; z++) {
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,
                     (x + z) % 2 == 0 ? GREEN : WHITE);
        glVertex3d(x, 0, z);
        glVertex3d(x-1, 0, z);
        glVertex3d(x-1, 0, z-1);
        glVertex3d(x, 0, z-1);
      }
    }
    glEnd();
    glEndList();
  }
  void draw() {
    glCallList(displayListId);
  }
};

// Camera Class
class Camera {
  double theta;      // determines the x and z positions
  double y;          // the current y position
  double dTheta;     // increment in theta for swinging the camera around
  double dy;         // increment in y for moving the camera up/down
public:
  Camera(): theta(0.5), y(1), dTheta(0.04), dy(0.2) {}
  double getX() {return 2 * cos(theta);}
  double getY() {return y;}
  double getZ() {return 4 * sin(theta);}
  void moveRight() {theta += dTheta;}
  void moveLeft() {theta -= dTheta;}
  void moveUp() {y += dy;}
  void moveDown() {if (y > dy) y -= dy;}
};

Checkerboard checkerboard(2,2);
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

void special(int key, int, int) {
  switch (key) {
    case GLUT_KEY_LEFT: camera.moveLeft(); break;
    case GLUT_KEY_RIGHT: camera.moveRight(); break;
    case GLUT_KEY_UP: camera.moveUp(); break;
    case GLUT_KEY_DOWN: camera.moveDown(); break;
  }
  glutPostRedisplay();
}

void reshape(GLint w, GLint h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (h == 0)
        h = 1;
    float ratio =  w * 1.0 / h;
    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);
    // Reset Matrix
    glLoadIdentity();
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    // Set the correct perspective.
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
}

void render() {
  /*
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 100.0f);
    // Camera matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around
    */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
      //  gluLookAt(camera.getX(), camera.getY(), camera.getZ(),
        //          checkerboard.centerx(), 0.0, checkerboard.centerz(),
          //        0.0, 1.0, 0.0);
  /*      gluLookAt (camera.getX(), camera.getY(), camera.getZ(),
                   0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0);*/
    checkerboard.draw();
    for (vector<mass>::iterator item = masses.begin(); item != masses.end(); item++) {
        item->draw();
    }
    for (vector<spring>::iterator item = springs.begin(); item != springs.end(); item++) {
        item->draw();
    }
    glFlush();
};


int main(int argc, char **argv) {
    // set global vars
    // initialize mass and array
    double weight = 0.1; //kg
    double k = 100000; //Nmss
    int fps = 60;
    double oneSecondOfSim = 1;
    oneSecondOfSim = 1/timestep;
    double simSteps = 1;
    simSteps = oneSecondOfSim/fps;
    double frameTime = 1;
    frameTime /= fps;
    //int steps = 500000;
    masses = generateMasses(weight);
    springs = generateSprings(k,masses,springs);
    linkMassSpring(masses,springs);
    
  /*  for (mass& m : masses) {
        m.moveMass(0, 0.5, 0);
    }
   */
    
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
    
    /* Loop until the user closes the window */
       while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 && t < 10)
       {
           /* Render here */
           render();
           /* Swap front and back buffers */
           glfwSwapBuffers(window);
           
           while (deltaTime <= frameTime) {
               simulate(false,1,true);
               time = glfwGetTime();
               deltaTime = time - lastTime;
           }
           
           deltaTime = time - lastTime;
           lastTime = glfwGetTime();
           /* Poll for and process events */
           glfwPollEvents();
          // glfwGetWindowSize(window, &width, &height);
          // reshape(width, height);
       }
    //
    glfwTerminate();
    
    return 0;
}

