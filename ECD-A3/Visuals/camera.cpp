//
//  camera.cpp
//  ECD-A3
//
//  Created by Salvy Cavicchio on 11/25/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#include "camera.hpp"

double Camera::getX() {return theta;}
double Camera::getY() {return y;}
double Camera::getZ() {return theta;}
void Camera::moveUp() {theta += dTheta;}
void Camera::moveDown() {theta -= dTheta;}
void Camera::moveRight() {y += dy;}
void Camera::moveLeft() {if (y > dy) y -= dy;}
