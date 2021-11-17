#include <ctime>
#include <glm/gtc/constants.hpp>
#include "AsteroidsGame.hpp"
#include "GameObject.hpp"
#include "SpriteComponent.hpp"
#include "SpriteAnimationComponent.hpp"
#include "Box2D/Dynamics/Contacts/b2Contact.h"
#include "PhysicsComponent.hpp"



using namespace sre;

const glm::vec2 AsteroidsGame::windowSize(1600,800);
std::shared_ptr<AsteroidsGame> AsteroidsGame::instance = nullptr;

AsteroidsGame::AsteroidsGame()
        :debugDraw(physicsScale){
    instance = std::shared_ptr<AsteroidsGame>(this);
    r.setWindowTitle("Asteroids");
    r.setWindowSize(windowSize);
    r.init().withSdlInitFlags(SDL_INIT_EVERYTHING)
            .withSdlWindowFlags(SDL_WINDOW_OPENGL);
    //Uint32 myEventType = SDL_RegisterEvents(1);
    time_t t;
    // random seed based on time
    srand((unsigned) time(&t));
    init();

    camera.setWindowCoordinates();

    r.frameUpdate = [&](float deltaTime){
        update(deltaTime);
    };

    r.keyEvent = [&](SDL_Event& event){
        keyEvent(event);
    };

    r.frameRender = [&](){
        render();
    };

    //restart();
    r.startEventLoop();
}

void AsteroidsGame::init(){
    if (world != nullptr){ // deregister call backlistener to avoid getting callbacks when recreating the world
        world->SetContactListener(nullptr);
    }

    toRemove.clear();
    sceneObjects.clear();
    physicsComponentLookup.clear();
    initPhysics();

    spriteAtlas = SpriteAtlas::create("asteroids.json","asteroids.png");
    score = 0;
    auto spaceshipSprite = spriteAtlas->get("playerShip1_blue.png");
    auto spaceship = createGameObject();
    //TODO: Add components to spaceship here
    /*for (size_t i = 0; i < 5; i++)
    {
        auto asteroid = createGameObject();
        //TODO: Add components to asteroids
    }*/

}

void AsteroidsGame::initPhysics() {
    //float gravity = -9.8; // 9.8 m/s2
    delete world;
    world = new b2World(b2Vec2(0,0));
    world->SetContactListener(this);

    if (doDebugDraw){
        world->SetDebugDraw(&debugDraw);
    }
}

void AsteroidsGame::update(float deltaTime) {
    if (gameState != GameState::Running)
        return;

    if (gameState == GameState::Running){
        updatePhysics();
        for(auto o: toRemove){
            auto found = std::find_if(sceneObjects.begin(), sceneObjects.end(), [&](std::shared_ptr<GameObject> obj){return obj.get() == o;});
            // if (found != sceneObjects.end())
            sceneObjects.erase(found);
        }
        toRemove.clear();
    }
    for (int i=0;i<sceneObjects.size();i++){
        sceneObjects[i]->update(deltaTime);
    }
}

void AsteroidsGame::updatePhysics() {
    const float32 timeStep = 1.0f / 60.0f;
    const int positionIterations = 2;
    const int velocityIterations = 6;
    world->Step(timeStep, velocityIterations, positionIterations);

    for (auto phys : physicsComponentLookup){
        if (phys.second->rbType == b2_staticBody) continue;
        auto position = phys.second->body->GetPosition();
        float angle = phys.second->body->GetAngle();
        auto gameObject = phys.second->getGameObject();
        gameObject->setPosition(glm::vec2(position.x*physicsScale, position.y*physicsScale));
        gameObject->setRotation(angle);
    }
}

void AsteroidsGame::render() {
    auto renderPass = RenderPass::create()
            .withCamera(camera)
            .withClearColor(true, {.20, .60, .86, 1})
            .build();
    auto spriteBatchBuilder = SpriteBatch::create();

    for (int i = 0; i < sceneObjects.size();i++) {
        sceneObjects[i]->renderSprite(spriteBatchBuilder);
    }

    auto spriteBatch = spriteBatchBuilder.build();
    renderPass.draw(spriteBatch);

    if (doDebugDraw){
        world->DrawDebugData();
        renderPass.drawLines(debugDraw.getLines());
        debugDraw.clear();
    }

    /*ImGui::SetNextWindowPos(ImVec2(Renderer::instance->getWindowSize().x/2 - 100, .0f), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, 70), ImGuiSetCond_Always);
    ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    ImGui::LabelText("GOs", "%i", (int)gameObjects.size());
    ImGui::LabelText("Score", "%i",score);
    ImGui::End();*/
}

void AsteroidsGame::keyEvent(SDL_Event &event) {
    for (auto & gameObject: sceneObjects) {
        for (auto & c : gameObject->getComponents()){
            bool consumed = c->keyEvent(event);
            if (consumed){
                return;
            }
        }
    }

    if (event.type == SDL_KEYDOWN){
        switch (event.key.keysym.sym){
            case SDLK_d:
                // press 'd' for physics debug
                doDebugDraw = !doDebugDraw;
                if (doDebugDraw){
                    world->SetDebugDraw(&debugDraw);
                } else {
                    world->SetDebugDraw(nullptr);
                }
                break;
            case SDLK_r:
                init();
                break;
            case SDLK_SPACE:
                if (gameState == GameState::GameOver){
                    init();
                    gameState = GameState::Ready;
                } else if (gameState == GameState::Ready){
                    gameState = GameState::Running;
                }
                break;
        }
    }
}

void AsteroidsGame::incrementScore() {
    score += 1;
}

std::shared_ptr<GameObject> AsteroidsGame::createGameObject() {
    auto obj = std::shared_ptr<GameObject>(new GameObject());
    sceneObjects.push_back(obj);
    return obj;
}

void AsteroidsGame::registerPhysicsComponent(PhysicsComponent *r) {
    physicsComponentLookup[r->fixture] = r;
}

void AsteroidsGame::deregisterPhysicsComponent(PhysicsComponent *r) {
    auto iter = physicsComponentLookup.find(r->fixture);
    if (iter != physicsComponentLookup.end()){
        physicsComponentLookup.erase(iter);
    } else {
        assert(false); // cannot find physics object
    }
}

void AsteroidsGame::removeObject(GameObject *object) {
    toRemove.push_back(object);
}

void AsteroidsGame::BeginContact(b2Contact *contact) {
    b2ContactListener::BeginContact(contact);
    handleContact(contact, true);
}

void AsteroidsGame::EndContact(b2Contact *contact) {
    b2ContactListener::EndContact(contact);
    handleContact(contact, false);
}

void AsteroidsGame::handleContact(b2Contact *contact, bool begin) {
    auto fixA = contact->GetFixtureA();
    auto fixB = contact->GetFixtureB();
    auto physA = physicsComponentLookup.find(fixA);
    auto physB = physicsComponentLookup.find(fixB);
    if (physA !=physicsComponentLookup.end() && physB != physicsComponentLookup.end()){
        auto & aComponents = physA->second->getGameObject()->getComponents();
        auto & bComponents = physB->second->getGameObject()->getComponents();
        for (auto & c : aComponents){
            if (begin){
                c->onCollisionStart(physB->second);
            } else {
                c->onCollisionEnd(physB->second);
            }
        }
        for (auto & c : bComponents){
            if (begin){
                c->onCollisionStart(physA->second);
            } else {
                c->onCollisionEnd(physA->second);
            }
        }
    }
}

void AsteroidsGame::setGameState(GameState newState) {
    this->gameState = newState;
}

int main(){
    new AsteroidsGame();
    return 0;
}