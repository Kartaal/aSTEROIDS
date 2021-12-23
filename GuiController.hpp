#pragma once

#include "Component.hpp"
#include <SDL_events.h>
#include <iostream>
#include "PlayerController.hpp"
#include "GameObject.hpp"
#include "AsteroidsGame.hpp"

/*
* A component handling GUI updates for score and weapon stats
*/

class GuiController : public Component {
public:
	explicit GuiController(GameObject* gameObject);								// The ctor is called with an object that has a WeaponComponent

	bool keyEvent(SDL_Event& event) override;

	void setFont(ImFont* font);

	void onGui() override;														// Draws GUI elements
private:
	void guiWeaponInfo();														// Draws the score and weapon info in a window
	ImFont* font;																// The font used to draw text with
};
