
#include "Component.hpp"

/*
* A component keeping track of the remaining life time of an object such as projectiles
*/

class LifetimeComponent: public Component {
public:
    explicit LifetimeComponent(GameObject *gameObject);

    void update(float deltaTime) override;                  // Updates timeToDeath and schedules the game object
                                                            //  for deletion if it is <= 0

    void setLifetime(float lifetime);
private:
    float timeToDeath;                                      // The remaining time to live in seconds

};