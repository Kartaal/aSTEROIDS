//
// Created by tta on 08/12/2021.
//

#include <AsteroidsGame.hpp>
#include "EnemyController.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"

EnemyController::EnemyController(GameObject *gameObject) : Component(gameObject) {

}

void EnemyController::update(float deltaTime) {
    Component::update(deltaTime);
}

void EnemyController::onCollisionStart(PhysicsComponent *comp) {
    auto collidedWithType = comp->getGameObject()->objectType;

    if (collidedWithType != ObjectType::PlayerBullet){
        return;
    }
    AsteroidsGame::instance->scheduleForRemoval(gameObject);
    auto type = this->gameObject->objectType;
    if (type == AsteroidLarge || type == AsteroidMedium){ //Will spawn two smaller steroids if there is a smaller size
        for (int i = 0; i < 2; ++i) {
            AsteroidsGame::instance->scheduleForCreation(ObjectType(type +1), glm::vec2(this->gameObject->getPosition()));
        }
    }
}

void EnemyController::onCollisionEnd(PhysicsComponent *comp) {
    Component::onCollisionEnd(comp);
}

