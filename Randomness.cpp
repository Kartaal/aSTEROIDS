//
// Created by Thomas Andersen on 30/09/2021.
//

#include "Randomness.h"
#include "AsteroidsGame.hpp"

#include <random>

int Randomness::generateRandomFloat(int maxOutput, int minOutput) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(minOutput, maxOutput); // define the range

    return distr(gen);
}

glm::vec2 Randomness::generateEnemySpawnPoint(float objectRadius) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 3); // define the range

    auto spawnDirection = distr(gen);
    int x;
    int y;
    if (spawnDirection == 0){ // spawn on the left of the screen
        x = -objectRadius;
        y = generateRandomFloat(AsteroidsGame::instance->windowSize.y);
    } else if (spawnDirection == 1){ //spawn at the top of the screen
        x = generateRandomFloat(AsteroidsGame::instance->windowSize.x);
        y = AsteroidsGame::instance->windowSize.y + objectRadius;
    } else if (spawnDirection == 2){ // spawn on the right of the screen
        x = AsteroidsGame::instance->windowSize.x + objectRadius;
        y = generateRandomFloat(AsteroidsGame::instance->windowSize.y);
    } else { // spawnDirection == 3, spawn at the bottom of the screen
        x = generateRandomFloat(AsteroidsGame::instance->windowSize.x);
        y = -objectRadius;
    }

    return glm::vec2{x, y};
}

glm::vec2 Randomness::generateSpawnDirection(glm::vec2 spawnPoint) {
    auto moveTowards = glm::vec2 {generateRandomFloat(AsteroidsGame::instance->windowSize.x),
                                   generateRandomFloat(AsteroidsGame::instance->windowSize.y)};

    auto direction = glm::normalize(spawnPoint-moveTowards);

    return direction;
}
