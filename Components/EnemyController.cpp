//
// Created by tta on 08/12/2021.
//

#include <AsteroidsGame.hpp>
#include "EnemyController.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"

EnemyController::EnemyController(GameObject* gameObject) : Component(gameObject) {

}

void EnemyController::update(float deltaTime) {
	Component::update(deltaTime);
}

void EnemyController::onCollisionStart(PhysicsComponent* comp) {
	auto collidedWithType = comp->getGameObject()->objectType;

	if (collidedWithType != ObjectType::PlayerBullet) {
		return;
	}

	// Play asteroid explosion sound
	AsteroidsGame::instance->playSound(1);

	// Schedule this object for removal
	AsteroidsGame::instance->scheduleForRemoval(gameObject);
	auto type = this->gameObject->objectType;

	//Will spawn two smaller steroids if there is a smaller size
	if (type == AsteroidLarge || type == AsteroidMedium) {
		for (int i = 0; i < 2; ++i) {
			AsteroidsGame::instance->scheduleForCreation(ObjectType(type + 1), glm::vec2(this->gameObject->getPosition()));
		}
	}
}

void EnemyController::onCollisionEnd(PhysicsComponent* comp) {
	Component::onCollisionEnd(comp);
}

