#include "AsteroidsGame.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "UpgradeController.hpp"
#include "WeaponComponent.h"
#include "Randomness.h"

UpgradeController::UpgradeController(GameObject* gameObject) : Component(gameObject)
{
	// Size of UpgradeType - 1
	auto randomType = Randomness::generateRandomFloat(3);

	type = UpgradeType(randomType);

	// Amount to upgrade by varies by upgrade type, some upgrades have different units
	switch (type)
	{
	case FireRate:
		value = 0.5f;
		break;
	case ProjectileSize:
		value = 0.5f;
		break;
	case ProjectileSpeed:
		value = 0.5f;
		break;
	case ProjectileLifetime:
		value = 1.0f;
		break;
	default:
		std::cout << "Upgrade type: " << type << " not covered in UpgradeController ctor" << std::endl;
		break;
	}
}

void UpgradeController::onCollisionStart(PhysicsComponent* comp)
{
	auto collidedWithType = comp->getGameObject()->objectType;

	// If collided with player, apply upgrade to player's weapon
	if (collidedWithType == PlayerShip) {
		auto weapComp = comp->getGameObject()->getComponent<WeaponComponent>();

		weapComp->applyUpgrade(type, value);

		AsteroidsGame::instance->scheduleForRemoval(gameObject);
	}
}

void UpgradeController::SetType(UpgradeType uType)
{
	type = uType;
}

void UpgradeController::SetValue(float uValue)
{
	value = uValue;
}

UpgradeType UpgradeController::GetType()
{
	return type;
}
