//
//  checkerboard.cpp
//  ECD-A3
//
//  Created by Salvy Cavicchio on 11/25/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#include "checkerboard.hpp"


void Checkerboard::create() {
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
    
    
void Checkerboard::draw() {
    glCallList(displayListId);
}

