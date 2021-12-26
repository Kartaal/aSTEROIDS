#include "Component.hpp"
#include "UpgradeType.hpp"

#ifndef SIMPLERENDERENGINEPROJECT_WEAPONCOMPONENT_HPP
#define SIMPLERENDERENGINEPROJECT_WEAPONCOMPONENT_HPP

/*
* Component handling the behaviour of a game object's weapon.
* This component is in itself the weapon of a game object
*  and as such tracks when it can fire, handling input for
*  firing, calling the AsteroidsGame to spawn in the projectile
*  and storing the stats of the weapon and applying upgrades.
*/

class WeaponComponent : public Component {
public:
	explicit WeaponComponent(GameObject* gameObject);
	void update(float deltaTime) override;

	bool keyEvent(SDL_Event& event) override;

	float getFireRate();
	float getProjectileSize();
	float getProjectileSpeed();
	float getProjectileLifetime();
	void applyUpgrade(UpgradeType type, float uValue);

private:
	//Upgradable values
	float fireRate = 3; //Projectiles per second
	float projectileSize = 1; //Percentage
	float projectileSpeed = 1; //Percentage
	float projectileLifetime = 1; //Seconds

	bool shooting = false;
	float timeSinceLastShot = 0;

	void shoot();
};

#endif //SIMPLERENDERENGINEPROJECT_WEAPONCOMPONENT_HPP
