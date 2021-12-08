#include "WeaponComponent.h"
#include <SDL_events.h>
#include "AsteroidsGame.hpp"
#include "LifetimeComponent.h"
#include "SpriteComponent.hpp"

WeaponComponent::WeaponComponent(GameObject *gameObject) : Component(gameObject) {
    shipPhysicsComp = gameObject->getComponent<PhysicsComponent>();

    fireSound = std::shared_ptr<Mix_Chunk>(Mix_LoadWAV("laserShoot.wav"));
}

void WeaponComponent::update(float deltaTime) {
    if(shooting && timeSinceLastShot >= 1.0f/fireRate){
        timeSinceLastShot = 0;
        shoot();
    }
    timeSinceLastShot += deltaTime;
}

void WeaponComponent::shoot(){
    AsteroidsGame::instance->SpawnProjectile(gameObject,projectileSize,projectileSpeed,projectileLifetime);
    Mix_PlayChannel(-1, fireSound.get(), 0);

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
