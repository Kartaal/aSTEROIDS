//
// Created by tta on 08/12/2021.
//

#ifndef SIMPLERENDERENGINEPROJECT_BULLETCONTROLLER_HPP
#define SIMPLERENDERENGINEPROJECT_BULLETCONTROLLER_HPP


#include "Component.hpp"

class BulletController  : public Component {
public:
    explicit BulletController(GameObject *gameObject);

    void update(float deltaTime) override;

    void onCollisionStart(PhysicsComponent *comp) override;

    void onCollisionEnd(PhysicsComponent *comp) override;
};


#endif //SIMPLERENDERENGINEPROJECT_BULLETCONTROLLER_HPP
