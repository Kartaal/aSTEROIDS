//
// Created by Thomas Andersen on 30/09/2021.
//

#ifndef SIMPLERENDERENGINEPROJECT_RANDOMNESS_H
#define SIMPLERENDERENGINEPROJECT_RANDOMNESS_H

#include <glm/vec2.hpp>

/*
* This static methods in this class are used for generating
*  certain randomised data including the type of upgrade to spawn.
*/

class Randomness {
public:
	static int generateRandomFloat(int maxOutput, int minOutput = 0);
	static glm::vec2 generateEnemySpawnPoint(float objectRadius);
	static glm::vec2 generateSpawnDirection(glm::vec2 spawnPoint);

};


#endif //SIMPLERENDERENGINEPROJECT_RANDOMNESS_H
