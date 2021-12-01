#include "WeaponComponent.h"
#include <SDL_events.h>
#include "AsteroidsGame.hpp"

WeaponComponent::WeaponComponent(GameObject *gameObject) : Component(gameObject) {

}

void WeaponComponent::update(float deltaTime) {
    if(shooting && timeSinceLastShot >= 1.0f/fireRate){
        timeSinceLastShot = 0;
        std::cout << "Pew pew look at me go" << std::endl;
    }
    timeSinceLastShot += deltaTime;
}

bool WeaponComponent::keyEvent(SDL_Event &event) {
    if (event.type == SDL_KEYDOWN){
        if (event.key.keysym.sym == SDLK_SPACE){
            shooting = true;
        }
    } else if (event.type == SDL_KEYUP){
        if (event.key.keysym.sym == SDLK_SPACE){
            shooting = false;
        }
    }

    return false;
}
