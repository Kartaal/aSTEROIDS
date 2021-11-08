#include <ctime>
#include <glm/gtc/constants.hpp>
#include "AsteroidsGame.hpp"
#include "GameObject.hpp"
#include "SpaceShip.hpp"
#include "Asteroid.hpp"
#include "Laser.h"
#include "Helper.hpp"


using namespace sre;

AsteroidsGame::AsteroidsGame() {
    r.setWindowTitle("Asteroids");

    r.init().withSdlInitFlags(SDL_INIT_EVERYTHING)
            .withSdlWindowFlags(SDL_WINDOW_OPENGL);


    //Uint32 myEventType = SDL_RegisterEvents(1);
    time_t t;
    // random seed based on time
    srand((unsigned) time(&t));

    atlas = SpriteAtlas::create("asteroids.json","asteroids.png");
    self = std::shared_ptr<AsteroidsGame>(this);

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

    restart();
    r.startEventLoop();
}


void AsteroidsGame::updatePhysics() {
    //TODO: Check if actualy correct
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

void AsteroidsGame::update(float deltaTime) {
    if (!gameRunning)
        return;

    for (int i = 0; i < gameObjects.size();i++) {
		gameObjects[i]->update(deltaTime);
        if(!gameObjects[i]->alive){
            gameObjects.erase(gameObjects.begin()+i);
        }
    }
    for (int i = 0; i < gameObjects.size(); ++i) {
        gameObjects[i]->update(deltaTime);
        if(!gameObjects[i]->alive){
            gameObjects.erase(gameObjects.begin()+i);
            continue;
        }
        for(int j = 0; j < gameObjects.size();j++){
            if(i==j){
                continue;
            }
            auto iobj = gameObjects[i];
            auto jobj = gameObjects[j];
            auto icol = std::dynamic_pointer_cast<Collidable>(iobj);
            auto jcol = std::dynamic_pointer_cast<Collidable>(jobj);

            if (icol == nullptr || jcol == nullptr)
                continue;
            if(glm::distance(iobj->position,jobj->position) <= icol->getRadius() + jcol->getRadius()) //Collision!
            {
                icol->onCollision(jobj);
                jcol->onCollision(iobj);
                }
            }
    }
}

void drawCircle(std::vector<glm::vec3>& lines, glm::vec2 position, float radius){
    int sides = 15;
    float quaterPi = glm::two_pi<float>() / (float)sides;
    for (float f = 0; f<glm::two_pi<float>(); f += quaterPi){
        // line from
        lines.push_back(glm::vec3{position.x + cosf(f)*radius,
                                  position.y + sinf(f)*radius,
                                  0
        });
        // line to
        lines.push_back(glm::vec3{position.x + cosf(f+quaterPi)*radius,
                                  position.y + sinf(f+quaterPi)*radius,
                                  0
        });
    }
}

void AsteroidsGame::render() {
    auto renderPass = RenderPass::create()
            .withCamera(camera)
            .withClearColor(true, {.20, .60, .86, 1})
            .build();
    auto spriteBatchBuilder = SpriteBatch::create();

    for (int i = 0; i < gameObjects.size();i++) {
        gameObjects[i]->render(spriteBatchBuilder);
    }
    auto spriteBatch = spriteBatchBuilder.build();
    renderPass.draw(spriteBatch);

    if (debugCollisionCircles){
        std::vector<glm::vec3> lines;
        for (auto & go : gameObjects){
            auto col = std::dynamic_pointer_cast<Collidable>(go);
            if (col != nullptr){
                drawCircle(lines, go->position, col->getRadius());
            }
        }
        renderPass.drawLines(lines);
    }

    ImGui::SetNextWindowPos(ImVec2(Renderer::instance->getWindowSize().x/2 - 100, .0f), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, 70), ImGuiSetCond_Always);
    ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    ImGui::LabelText("GOs", "%i", (int)gameObjects.size());
    ImGui::LabelText("Score", "%i",score);
    ImGui::End();
}

void AsteroidsGame::keyEvent(SDL_Event &event) {
    if (!gameRunning && event.key.keysym.sym == SDLK_SPACE){
        restart();
        return;
    }
    for (int i = 0; i < gameObjects.size();i++) {
        gameObjects[i]->onKey(event);
    }
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_d) {
        debugCollisionCircles = !debugCollisionCircles;
    }
}

sre::Sprite AsteroidsGame::getSprite(std::string spriteName) {
    return atlas->get(spriteName);
}

void AsteroidsGame::addGameObject(std::shared_ptr<GameObject> object) {
    gameObjects.push_back(object);
}

void AsteroidsGame::incrementScore() {
    score += 1;
}

void AsteroidsGame::restart(){
    gameObjects.clear();
    score = 0;
    auto spaceshipSprite = atlas->get("playerShip1_blue.png");
    auto spaceship = std::make_shared<SpaceShip>(spaceshipSprite,self);
    addGameObject(spaceship);
    for (size_t i = 0; i < 5; i++)
    {
        auto asteroid = std::make_shared<Asteroid>(self,large);
        addGameObject(asteroid);
    }
    gameRunning = true;
}

void AsteroidsGame::GameOver(){
    gameRunning = false;
}

int main(){
    new AsteroidsGame();
    return 0;
}