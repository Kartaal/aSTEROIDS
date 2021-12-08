//
// Created by tta on 08/12/2021.
//

#include <AsteroidsGame.hpp>
#include "BulletController.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"

BulletController::BulletController(GameObject *gameObject) : Component(gameObject) {

}

void BulletController::update(float deltaTime) {
    Component::update(deltaTime);
}

void BulletController::onCollisionStart(PhysicsComponent *comp) {
    auto collidedWithType = comp->getGameObject()->objectType;
    if (collidedWithType == AsteroidSmall || collidedWithType == AsteroidMedium || collidedWithType == AsteroidLarge){
        AsteroidsGame::instance->scheduleForRemoval(gameObject);
    }
}

void BulletController::onCollisionEnd(PhysicsComponent *comp) {
    Component::onCollisionEnd(comp);
}

