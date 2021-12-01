
#include "Component.hpp"

class LifetimeComponent: public Component {
public:
    explicit LifetimeComponent(GameObject *gameObject);
    void update(float deltaTime) override;

    void setLifetime(float lifetime);
private:
    float timeToDeath;

};