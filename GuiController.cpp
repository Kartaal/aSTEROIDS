#include "GuiController.hpp"
#include "sre/Renderer.hpp"
#include "WeaponComponent.h"
using namespace sre;

GuiController::GuiController(GameObject* gameObject) : Component(gameObject) {
	playerController = gameObject->getComponent<PlayerController>();
}


bool GuiController::keyEvent(SDL_Event& keyEvent) {

	if (keyEvent.type == SDL_KEYDOWN) {
		if (keyEvent.key.keysym.sym == SDLK_UP) {

		}
	}
	else if (keyEvent.type == SDL_KEYUP) {
		if (keyEvent.key.keysym.sym == SDLK_UP) {

		}
	}
	return false;
}

void GuiController::onGui() {
    guiHealth();
    guiWeaponInfo();
}

void GuiController::guiWeaponInfo() {
    auto r = Renderer::instance;
    auto winsize = r->getWindowSize();
    ImVec2 size = {300, 150};
    ImVec2 pos = {winsize.x - size.x,0};
    auto cond = ImGuiCond_Always;
    ImVec2 pivot = {0,0};
    ImGui::SetNextWindowPos(pos, cond, pivot);

    ImGui::SetNextWindowSize(size, cond);
    auto flags =
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar;
    bool* open = nullptr;
    ImGui::Begin("#gameinfo", open, flags);
    ImGui::PushFont(font);

    auto weaponComponent = gameObject->getComponent<WeaponComponent>();
    // draw Score
    ImGui::PushID(1);
    auto scoreStr = std::to_string(AsteroidsGame::instance->getScore());
    ImGui::Text("Score"); ImGui::SameLine();
    float width = ImGui::CalcTextSize(scoreStr.c_str()).x;
    float windowWidth = ImGui::GetWindowContentRegionWidth();
    ImGui::SetCursorPosX(windowWidth - width); // align right
    ImGui::Text("%d", AsteroidsGame::instance->getScore());
    ImGui::Text("Fire rate");ImGui::SameLine();
    ImGui::Text("%f",weaponComponent->getFireRate());
    ImGui::Text("Projectile size");ImGui::SameLine();
    ImGui::Text("%f",weaponComponent->getProjectileSize());
    ImGui::Text("Projectile speed");ImGui::SameLine();
    ImGui::Text("%f",weaponComponent->getProjectileSpeed());
    ImGui::Text("Projectile lifetime");ImGui::SameLine();
    ImGui::Text("%f",weaponComponent->getProjectileSize());

    ImGui::PopID();

    /*
    // Draw powerbar
    ImGui::Text("Power"); ImGui::SameLine();
    width = powerbarSize.x;
    ImGui::SetCursorPosX(windowWidth - width); // align right
    // Draw background
    ImGui::Image(powerbar->getNativeTexturePtr(),{powerbarSize.x,powerbarSize.y}, uv0, uv1); ImGui::SameLine();
    float border=3;
    auto innerSize = powerbarSize - glm::vec2(border*2,border*2);
    ImGui::SetCursorPosX(windowWidth - width + border); // align right
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + border); // move down
    // scale/clip inner bar
    innerSize.x *= power;
    uv1.x *= power;
    ImVec4 tintColor(0,1,0,1);
    ImGui::Image(powerbar->getNativeTexturePtr(),{innerSize.x,innerSize.y}, uv0, uv1, tintColor);
    */
    ImGui::PopFont();
    ImGui::End();

}

void GuiController::guiHealth() {

}

void GuiController::setPlayerController(std::shared_ptr<PlayerController> controller) {
    playerController = controller;
}

void GuiController::setFont(ImFont *font) {
    this->font = font;
}

