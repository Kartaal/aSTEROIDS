#pragma once

#include "Component.hpp"

/*
* A component handling behaviour of a player object
*/

class PlayerController : public Component {
public:
	explicit PlayerController(GameObject* gameObject);

	void update(float deltaTime) override;						// Handles updating the position, rotation and adding forces

	bool keyEvent(SDL_Event& event) override;

	void onCollisionStart(PhysicsComponent* comp) override;		// Handles collisions starting, checking if comp is an asteroid
																//  if not, leave updating to physics simulation
																// On collision with an asteroid, play appropriate sound,
																//  set the game state to GameOver and replace the sprite for a death sprite

	void onCollisionEnd(PhysicsComponent* comp) override;
private:
	std::shared_ptr<PhysicsComponent> physicsComp;

	bool rotateCW = false;										// If currently rotating clock-wise
	bool rotateCCW = false;										// If currently rotating counter-clock-wise
	bool thrusting = false;										// If currently thrusting
										
	float thrustPower = 100;									// How fast it should move
	float rotationSpeed = 10000;								// How fast should it rotate 
																//  very high to account for degrees to radians calculation
};
