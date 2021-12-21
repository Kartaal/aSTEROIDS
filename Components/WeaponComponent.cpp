#include "WeaponComponent.h"
#include <SDL_events.h>
#include "AsteroidsGame.hpp"
#include "LifetimeComponent.h"
#include "SpriteComponent.hpp"

WeaponComponent::WeaponComponent(GameObject* gameObject) : Component(gameObject) {
	shipPhysicsComp = gameObject->getComponent<PhysicsComponent>();
}

void WeaponComponent::update(float deltaTime) {
	if (shooting && timeSinceLastShot >= 1.0f / fireRate) {
		timeSinceLastShot = 0;
		shoot();
	}
	timeSinceLastShot += deltaTime;
}

void WeaponComponent::shoot() {
	AsteroidsGame::instance->SpawnProjectile(gameObject, projectileSize, projectileSpeed, projectileLifetime);
	AsteroidsGame::instance->playSound(Shooting);

}

bool WeaponComponent::keyEvent(SDL_Event& event) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_SPACE) {
			shooting = true;
		}
	}
	else if (event.type == SDL_KEYUP) {
		if (event.key.keysym.sym == SDLK_SPACE) {
			shooting = false;
		}
	}

	return false;
}

float WeaponComponent::getFireRate() {
    return fireRate;
}

float WeaponComponent::getProjectileSize() {
    return projectileSize;
}

float WeaponComponent::getProjectileSpeed() {
    return projectileSpeed;
}

float WeaponComponent::getProjectileLifetime() {
    return projectileLifetime;
}
