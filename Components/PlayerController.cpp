//
// Created by Morten Nobel-Jørgensen on 19/10/2017.
//
#define GLM_ENABLE_EXPERIMENTAL
#include <SDL_events.h>
#include <iostream>
#include <glm/gtx/rotate_vector.hpp>
#include "PlayerController.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "AsteroidsGame.hpp"

PlayerController::PlayerController(GameObject *gameObject) : Component(gameObject) {
    physicsComp = gameObject->getComponent<PhysicsComponent>();

}

bool PlayerController::keyEvent(SDL_Event &keyEvent) {

    if (keyEvent.type == SDL_KEYDOWN){
        if (keyEvent.key.keysym.sym == SDLK_UP){
            thrusting = true;
        }
        if (keyEvent.key.keysym.sym == SDLK_LEFT){
            rotateCCW = true;
        }
        if (keyEvent.key.keysym.sym == SDLK_RIGHT){
            rotateCW = true;
        }
    } else if (keyEvent.type == SDL_KEYUP){
        if (keyEvent.key.keysym.sym == SDLK_UP){
            thrusting = false;
        }
        if (keyEvent.key.keysym.sym == SDLK_LEFT){
            rotateCCW = false;
        }
        if (keyEvent.key.keysym.sym == SDLK_RIGHT){
            rotateCW = false;
        }
    }

    return false;
}

void PlayerController::onCollisionStart(PhysicsComponent *comp) {
    auto collidedWithName = comp->getGameObject()->name;
    //std::cout << "bird collided with "<< collidedWithName << std::endl;
    /* //Example code, do not use
    if (collidedWithName == "Wall bottom" || collidedWithName == "Wall top")
        AsteroidsGame::instance->setGameState(GameState::GameOver);

    if (collidedWithName == "Coin"){
        auto coin = comp->getGameObject();
        AsteroidsGame::instance->removeObject(coin);
    } */
}

void PlayerController::onCollisionEnd(PhysicsComponent *comp) {

}

void PlayerController::update(float deltaTime) {
    auto rotation = physicsComp->getRotation();
    auto position = gameObject->getPosition();

    if (thrusting) {
        float acceleration = deltaTime * thrustPower;
        glm::vec2 direction = glm::rotateZ(glm::vec3(0, acceleration, 0), glm::radians(rotation));

        physicsComp->addForce(direction);
    }

    if (rotateCCW){
        rotation += glm::radians( deltaTime * rotationSpeed);
    }
    if (rotateCW){
        rotation -= glm::radians(deltaTime * rotationSpeed);
    }
    physicsComp->setRotation(rotation);
}


