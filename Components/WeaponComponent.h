#include "Component.hpp"

class WeaponComponent : public Component {
public:
	explicit WeaponComponent(GameObject* gameObject);
	void update(float deltaTime) override;

	bool keyEvent(SDL_Event& event) override;

	void applyUpgrade(UpgradeType, float);

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
