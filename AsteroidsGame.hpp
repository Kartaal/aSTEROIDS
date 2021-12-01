#pragma once

#include <vector>
#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"
#include "Box2D/Dynamics/b2World.h"
#include "GameObject.hpp"
//#include "BackgroundComponent.hpp"
#include "Box2DDebugDraw.hpp"
#include "Component.hpp"
#include "PhysicsComponent.hpp"

class GameObject;
class PhysicsComponent;
enum class GameState{
    Ready,
    Running,
    GameOver
};
class AsteroidsGame : public b2ContactListener{
public:
    AsteroidsGame();
    static const glm::vec2 windowSize;
    static const glm::vec2 AsteroidsGame::wrapperSize;
    //sre::Sprite getSprite(std::string spriteName);
    static std::shared_ptr<AsteroidsGame> instance;
    std::shared_ptr<GameObject> createGameObject();

    void incrementScore();
    //void GameOver();
    void BeginContact(b2Contact *contact) override;
    void EndContact(b2Contact *contact) override;

    void setGameState(GameState newState);

    void removeObject(GameObject *obj);

private:
    void init();
    void initPhysics();

    void update(float deltaTime);
    void render();
    void keyEvent(SDL_Event &event);
    void handleContact(b2Contact *contact, bool begin);
  //  void otherEvent(SDL_Event &event);
    sre::Camera camera;
    std::shared_ptr<sre::SpriteAtlas> spriteAtlas;

    std::vector<std::shared_ptr<GameObject>> sceneObjects;
    //sre::Camera camera;
    sre::SDLRenderer r;
    //std::shared_ptr<sre::SpriteAtlas> atlas;
    //std::vector<std::shared_ptr<GameObject>> gameObjects;
    int score = 0;
    //void restart();

    void updatePhysics();
    b2World * world = nullptr;
    static const float physicsScale;
    void registerPhysicsComponent(PhysicsComponent *r);
    void deregisterPhysicsComponent(PhysicsComponent *r);
    std::map<b2Fixture*,PhysicsComponent *> physicsComponentLookup;
    Box2DDebugDraw debugDraw;
    bool doDebugDraw = false;
    GameState gameState = GameState::Running;
    friend class PhysicsComponent;
    std::vector<GameObject*> toRemove;
};


