#include "Component.hpp"

class WeaponComponent: public Component {
public:
    explicit WeaponComponent(GameObject *gameObject);
    void update(float deltaTime) override;

    bool keyEvent(SDL_Event &event) override;

private:
    //Upgradable values
    float fireRate = 3; //Projectiles per second
    float projectileSize = 1; //Percentage
    float projectileSpeed = 1; //Percentage

    bool shooting = false;
    float timeSinceLastShot = 0;
};
