#pragma once

#include <vector>
#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"
#include "Box2D/Dynamics/b2World.h"
#include "GameObject.hpp"
#include "ObjectType.hpp"
//#include "BackgroundComponent.hpp"
#include "Box2DDebugDraw.hpp"
#include "Component.hpp"
#include "PhysicsComponent.hpp"
#include <SDL_mixer.h>

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
    static const glm::vec2 wrapperSize;
    //sre::Sprite getSprite(std::string spriteName);
    std::shared_ptr<sre::SpriteAtlas> spriteAtlas;
    static std::shared_ptr<AsteroidsGame> instance;
    std::shared_ptr<GameObject> createGameObject();
    std::shared_ptr<GameObject> SpawnEnemy(ObjectType, glm::vec2 position= glm::vec2{0,0});
    std::shared_ptr<GameObject> SpawnProjectile(GameObject* shooter, float projectileSize,
        float projectileSpeed, float projectileLifetime);

    void incrementScore();
    //void GameOver();
    void BeginContact(b2Contact *contact) override;
    void EndContact(b2Contact *contact) override;

    void setGameState(GameState newState);

    void scheduleForRemoval(GameObject *obj);
    void scheduleForCreation(ObjectType type, glm::vec2 position);

    void playSound(int soundIndex);

private:
    void init();
    void initPhysics();

    void update(float deltaTime);
    void render();
    void keyEvent(SDL_Event &event);
    void handleContact(b2Contact *contact, bool begin);
  //  void otherEvent(SDL_Event &event);
    sre::Camera camera;

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
    std::vector<std::pair<ObjectType, glm::vec2>> toCreate;

    float spawnTimerReset = 2.0f;
    float spawnTimer = 2.0f;

    // Sound references
    std::shared_ptr<Mix_Chunk> asteroidExplosionSound;
    std::shared_ptr<Mix_Chunk> deathExplosion;
    std::shared_ptr<Mix_Chunk> fireSound;
};


