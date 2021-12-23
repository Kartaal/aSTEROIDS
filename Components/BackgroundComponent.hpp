#pragma once

#include "Component.hpp"
#include "sre/SpriteBatch.hpp"
#include "sre/Texture.hpp"
#include "sre/RenderPass.hpp"

/*
* Currently not implemented component for the background of the game.
*/

class BackgroundComponent {
public:
    BackgroundComponent();
    void init(std::string filename);
    void renderBackground(sre::RenderPass& renderPass, float offset);
private:
    std::shared_ptr<sre::Texture> tex;
    std::shared_ptr<sre::SpriteBatch> batch;
};
