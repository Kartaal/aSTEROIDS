#include "WeaponComponent.h"
#include <SDL_events.h>
#include "AsteroidsGame.hpp"
#include "LifetimeComponent.h"
#include "SpriteComponent.hpp"
#include <UpgradeType.hpp>

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

void WeaponComponent::applyUpgrade(UpgradeType type, float uValue)
{
	switch (type)
	{
	case FireRate:
		fireRate += uValue;
		break;
	case ProjectileSize:
		projectileSize += uValue;
		break;
	case ProjectileSpeed:
		projectileSpeed += uValue;
		break;
	case ProjectileLifetime:
		projectileLifetime += uValue;
		break;
	default:
		std::cout << "Upgrade type: " << type << " not covered in applyUpgrade" << std::endl;
		break;
	}
}
