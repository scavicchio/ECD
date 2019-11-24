//
//  init.hpp
//  ECD-A3
//
//  Created by Salvy Cavicchio on 11/13/19.
//  Copyright © 2019 Salvy. All rights reserved.
//

#ifndef init_hpp
#define init_hpp

#include <stdio.h>
#include <vector>
#include "mass.hpp"
#include "spring.hpp"

std::vector<mass> generateMasses(double weight);

std::vector<spring>& generateSprings(double k, std::vector<mass> &masses, std::vector<spring>& theResult);


void linkMassSpring(std::vector<mass>& masses, std::vector<spring>& springs);

#endif /* init_hpp */
