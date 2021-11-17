#pragma once

#include "Component.hpp"

class PlayerController : public Component {
public:
    explicit PlayerController(GameObject *gameObject);

    bool keyEvent(SDL_Event &event) override;

    void onCollisionStart(PhysicsComponent *comp) override;

    void onCollisionEnd(PhysicsComponent *comp) override;
private:
    bool spacePressed = false;
};
