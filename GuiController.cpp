#include "GuiController.hpp"
#include <sre/Renderer.hpp>

GuiController::GuiController(GameObject* gameObject) : Component(gameObject) {
	playerController = gameObject->getComponent<PlayerController>();

    auto fonts = ImGui::GetIO().Fonts;
    fonts->AddFontDefault();
    auto fontName = "Assets/Fastrace.ttf";
    int fontSize = 20;
    FastRace =
            fonts->AddFontFromFileTTF(fontName, fontSize);

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
    ImVec2 size = {180, 107};
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
    ImGui::PushFont(FastRace);

    // draw Score
    ImGui::PushID(1);
    auto scoreStr = std::to_string(AsteroidsGame::instance->getScore());
    ImGui::Text("Score"); ImGui::SameLine();
    float width = ImGui::CalcTextSize(scoreStr.c_str()).x;
    float windowWidth = ImGui::GetWindowContentRegionWidth();
    ImGui::SetCursorPosX(windowWidth - width); // align right
    ImGui::Text(scoreStr.c_str());
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
