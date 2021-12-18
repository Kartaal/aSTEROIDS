#ifndef SIMPLERENDERENGINEPROJECT_UPGRADECONTROLLER_HPP
#define SIMPLERENDERENGINEPROJECT_UPGRADECONTROLLER_HPP

#include "Component.hpp"
#include "UpgradeType.hpp"

class UpgradeController : public Component {
public:
	explicit UpgradeController(GameObject* gameObject);	

	void onCollisionStart(PhysicsComponent* comp) override;

	void SetType(UpgradeType uType);

	void SetValue(float uValue);

	UpgradeType GetType();

private:
	UpgradeType type;
	float value;
};


#endif //SIMPLERENDERENGINEPROJECT_UPGRADECONTROLLER_HPP
