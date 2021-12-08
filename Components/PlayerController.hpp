#pragma once

#include "Component.hpp"
#include <SDL_mixer.h>

class PlayerController : public Component {
public:
    explicit PlayerController(GameObject *gameObject);

    void update(float deltaTime) override;

    bool keyEvent(SDL_Event &event) override;

    void onCollisionStart(PhysicsComponent *comp) override;

    void onCollisionEnd(PhysicsComponent *comp) override;
private:
    std::shared_ptr<PhysicsComponent> physicsComp;

    bool rotateCW = false;
    bool rotateCCW = false;
    bool thrusting = false;

    float drag = 0.8f;
    float thrustPower = 100;
    float maxSpeed;
    float rotationSpeed = 10000;

    std::shared_ptr<Mix_Chunk> deathExplosion;
};
