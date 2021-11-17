//
// Created by Morten Nobel-Jørgensen on 19/10/2017.
//

#include <SDL_events.h>
#include <iostream>
#include "PlayerController.hpp"
#include "GameObject.hpp"
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "AsteroidsGame.hpp"
#include "SpriteComponent.hpp"

PlayerController::PlayerController(GameObject *gameObject) : Component(gameObject) {
    //gameObject->getComponent<PhysicsComponent>()->setLinearVelocity(glm::vec2(1,0));
}

bool PlayerController::keyEvent(SDL_Event &event) {

    if (keyEvent.key.keysym.sym == SDLK_UP){
        thrust = keyEvent.type == SDL_KEYDOWN;
    }
    if (keyEvent.key.keysym.sym == SDLK_LEFT){
        rotateCCW = keyEvent.type == SDL_KEYDOWN;
    }
    if (keyEvent.key.keysym.sym == SDLK_RIGHT){
        rotateCW = keyEvent.type == SDL_KEYDOWN;
    }
    if (keyEvent.key.keysym.sym == SDLK_SPACE){
        Shoot();
    }
    /*
    if (event.type == SDL_KEYDOWN){
//        std::cout << "some key pressed" << std::endl;

        if(event.key.keysym.sym == SDLK_SPACE && !spacePressed){
            gameObject->getComponent<PhysicsComponent>()->addImpulse(glm::vec2(0,0.15));
            spacePressed = true;
        }
    } else if (event.type == SDL_KEYUP){
  //      std::cout << "some key released" << std::endl;
        if(event.key.keysym.sym == SDLK_SPACE && spacePressed){
            spacePressed = false;
        }
    }
     */
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


