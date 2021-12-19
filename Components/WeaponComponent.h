#include "Component.hpp"
#include "UpgradeType.hpp"

//#ifndef SIMPLERENDERENGINEPROJECT_WEAPONCOMPONENT_HPP
//#define SIMPLERENDERENGINEPROJECT_WEAPONCOMPONENT_HPP

class WeaponComponent : public Component {
public:
	explicit WeaponComponent(GameObject* gameObject);
	void update(float deltaTime) override;

	bool keyEvent(SDL_Event& event) override;

	void applyUpgrade(UpgradeType type, float uValue);

private:
	std::shared_ptr<PhysicsComponent> shipPhysicsComp;
	//Upgradable values
	float fireRate = 3; //Projectiles per second
	float projectileSize = 1; //Percentage
	float projectileSpeed = 1; //Percentage
	float projectileLifetime = 1; //Seconds

	bool shooting = false;
	float timeSinceLastShot = 0;

	void shoot();
};

//#endif //SIMPLERENDERENGINEPROJECT_WEAPONCOMPONENT_HPP