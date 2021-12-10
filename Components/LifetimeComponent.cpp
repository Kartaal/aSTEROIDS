#include "LifetimeComponent.h"
#include "GameObject.hpp"
#include "AsteroidsGame.hpp"

LifetimeComponent::LifetimeComponent(GameObject *gameObject) : Component(gameObject) {

}

void LifetimeComponent::update(float deltaTime) {
    timeToDeath -= deltaTime;
    if (timeToDeath <= 0){
        AsteroidsGame::instance->scheduleForRemoval(gameObject);
    }
}

void LifetimeComponent::setLifetime(float lifetime) {
    timeToDeath = lifetime;
}
