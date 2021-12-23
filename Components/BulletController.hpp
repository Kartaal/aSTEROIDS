//
// Created by tta on 08/12/2021.
//

#ifndef SIMPLERENDERENGINEPROJECT_BULLETCONTROLLER_HPP
#define SIMPLERENDERENGINEPROJECT_BULLETCONTROLLER_HPP

#include "Component.hpp"

/*
* Component handling updating and collisions for projectiles
*/

class BulletController : public Component {
public:
	explicit BulletController(GameObject* gameObject);

	void update(float deltaTime) override;

	void onCollisionStart(PhysicsComponent* comp) override;		// Handles the start of collisions and 
																//  tests for whether comp belongs to an asteroid
																//  if not an asteroid, there is no need to handle the collision

	void onCollisionEnd(PhysicsComponent* comp) override;
};


#endif //SIMPLERENDERENGINEPROJECT_BULLETCONTROLLER_HPP
