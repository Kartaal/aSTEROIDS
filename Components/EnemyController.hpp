//
// Created by tta on 08/12/2021.
//

#ifndef SIMPLERENDERENGINEPROJECT_ENEMYCONTROLLER_HPP
#define SIMPLERENDERENGINEPROJECT_ENEMYCONTROLLER_HPP

#include "Component.hpp"

/*
* A component controlling the behaviour of enemies (asteroids)
*/

class EnemyController : public Component {
public:
	explicit EnemyController(GameObject* gameObject);

	void update(float deltaTime) override;

	void onCollisionStart(PhysicsComponent* comp) override;		// Handles initial collisions with other objects
																// If the collided object is not a player projectile,
																//  return to only do physics calculations.
																// If colliding with a player projectile,
																//  play the appropriate sound, 
																//  increment score,
																//  mark this object for deletion and
																//  spawn two smaller asteroids (if this isn't the smallest type)

	void onCollisionEnd(PhysicsComponent* comp) override;
};


#endif //SIMPLERENDERENGINEPROJECT_ENEMYCONTROLLER_HPP
