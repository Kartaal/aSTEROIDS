#define GLM_ENABLE_EXPERIMENTAL
#include <SDL_events.h>
#include <iostream>
#include <glm/gtx/rotate_vector.hpp>
#include "PlayerController.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "AsteroidsGame.hpp"
#include "SpriteComponent.hpp"

PlayerController::PlayerController(GameObject* gameObject) : Component(gameObject) {
	physicsComp = gameObject->getComponent<PhysicsComponent>();

}

bool PlayerController::keyEvent(SDL_Event& keyEvent) {

	if (keyEvent.type == SDL_KEYDOWN) {
		if (keyEvent.key.keysym.sym == SDLK_UP) {
			thrusting = true;
		}
		if (keyEvent.key.keysym.sym == SDLK_LEFT) {
			rotateCCW = true;
		}
		if (keyEvent.key.keysym.sym == SDLK_RIGHT) {
			rotateCW = true;
		}
	}
	else if (keyEvent.type == SDL_KEYUP) {
		if (keyEvent.key.keysym.sym == SDLK_UP) {
			thrusting = false;
		}
		if (keyEvent.key.keysym.sym == SDLK_LEFT) {
			rotateCCW = false;
		}
		if (keyEvent.key.keysym.sym == SDLK_RIGHT) {
			rotateCW = false;
		}
	}

	return false;
}

void PlayerController::onCollisionStart(PhysicsComponent* comp) {
	auto collidedWithType = comp->getGameObject()->objectType;

	if (collidedWithType == AsteroidSmall || collidedWithType == AsteroidMedium || collidedWithType == AsteroidLarge) {

		// Play spaceship explosion sound
		AsteroidsGame::instance->playSound(SpaceshipExplosion);

		// Change game state and update sprite to bang
		AsteroidsGame::instance->setGameState(GameState::GameOver);
		auto spriteComp = this->getGameObject()->getComponent<SpriteComponent>();
		auto sprite = AsteroidsGame::instance->spriteAtlas->get("bang.png");
		spriteComp->setSprite(sprite);
	}

}

void PlayerController::onCollisionEnd(PhysicsComponent* comp) {

}

void PlayerController::update(float deltaTime) {
	auto rotation = physicsComp->getRotation();
	auto position = gameObject->getPosition();

	if (thrusting) {
		float acceleration = deltaTime * thrustPower;
		glm::vec2 direction = glm::rotateZ(glm::vec3(0, acceleration, 0), glm::radians(rotation));

		physicsComp->addForce(direction);
	}

	if (rotateCCW) {
		rotation += glm::radians(deltaTime * rotationSpeed);
	}
	if (rotateCW) {
		rotation -= glm::radians(deltaTime * rotationSpeed);
	}
	physicsComp->setRotation(rotation);
}

void PlayerController::guiWeaponInfo() {

}

void PlayerController::guiHealth() {

}

void PlayerController::onGui() {

}
