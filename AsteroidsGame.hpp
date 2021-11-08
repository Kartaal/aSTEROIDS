#pragma once

#include <vector>
#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"
#include "GameObject.hpp"
#include "Collidable.hpp"

class GameObject;
class CollidableGameObject;
class AsteroidsGame {
public:
    AsteroidsGame();
    sre::Sprite getSprite(std::string spriteName);
    void addGameObject(std::shared_ptr<GameObject> object);
    void incrementScore();
    void GameOver();

private:
    void update(float deltaTime);
    void render();
    void keyEvent(SDL_Event &event);
    std::shared_ptr<AsteroidsGame> self;
  //  void otherEvent(SDL_Event &event);

    sre::Camera camera;
    sre::SDLRenderer r;
    std::shared_ptr<sre::SpriteAtlas> atlas;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    bool debugCollisionCircles = false;
    int score = 0;
    bool gameRunning;
    void restart();
};


