#pragma once

#include "Component.hpp"
#include <SDL_events.h>
#include <iostream>
#include "PlayerController.hpp"
#include "GameObject.hpp"
#include "AsteroidsGame.hpp"

class GuiController : public Component {
public:
	explicit GuiController(GameObject* gameObject);

	bool keyEvent(SDL_Event& event) override;

    void setPlayerController(std::shared_ptr<PlayerController> controller);

    void onGui() override;
private:
    void guiWeaponInfo();
    void guiHealth();
    ImFont* FastRace;
	std::shared_ptr<PlayerController> playerController;
};
