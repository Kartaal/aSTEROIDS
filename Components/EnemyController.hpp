//
// Created by tta on 08/12/2021.
//

#ifndef SIMPLERENDERENGINEPROJECT_ENEMYCONTROLLER_HPP
#define SIMPLERENDERENGINEPROJECT_ENEMYCONTROLLER_HPP


#include "Component.hpp"

class EnemyController  : public Component {
public:
    explicit EnemyController(GameObject *gameObject);

    void update(float deltaTime) override;

    void onCollisionStart(PhysicsComponent *comp) override;

    void onCollisionEnd(PhysicsComponent *comp) override;
};


#endif //SIMPLERENDERENGINEPROJECT_ENEMYCONTROLLER_HPP
