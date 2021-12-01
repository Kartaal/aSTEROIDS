//
// Created by Thomas Andersen on 30/09/2021.
//

#include "RandomFloat.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

float RandomFloat::generateRandomFloat(int maxOutput) {
    float generated = float(rand())/float((RAND_MAX)) * maxOutput;
    return generated;
}
