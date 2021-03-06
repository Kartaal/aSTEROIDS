#include <ctime>
#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <EnemyController.hpp>
#include <BulletController.hpp>
#include "AsteroidsGame.hpp"
#include "GameObject.hpp"
#include "ObjectType.hpp"
#include "SpriteComponent.hpp"
#include "PlayerController.hpp"
#include "SpriteAnimationComponent.hpp"
#include "Box2D/Dynamics/Contacts/b2Contact.h"
#include "PhysicsComponent.hpp"
#include "WeaponComponent.h"
#include "GuiController.hpp"
#include "LifetimeComponent.h"
#include "Randomness.h"
#include <SDL_mixer.h>
#include "UpgradeController.hpp"


using namespace sre;

const float AsteroidsGame::physicsScale = 100;
const glm::vec2 AsteroidsGame::windowSize(1600, 800);
const glm::vec2 AsteroidsGame::wrapperSize = windowSize / physicsScale;
std::shared_ptr<AsteroidsGame> AsteroidsGame::instance = nullptr;

AsteroidsGame::AsteroidsGame()
	:debugDraw(physicsScale) {
	instance = std::shared_ptr<AsteroidsGame>(this);
	r.setWindowTitle("Asteroids");
	r.setWindowSize(windowSize);
	r.init().withSdlInitFlags(SDL_INIT_EVERYTHING)
		.withSdlWindowFlags(SDL_WINDOW_OPENGL);

	time_t t;
	// random seed based on time
	srand((unsigned)time(&t));

	setupSounds();
	initGui();
	init();


	camera.setWindowCoordinates();

	r.frameUpdate = [&](float deltaTime) {
		update(deltaTime);
	};

	r.keyEvent = [&](SDL_Event& event) {
		keyEvent(event);
	};

	r.frameRender = [&]() {
		render();
	};

	r.startEventLoop();
}

void AsteroidsGame::init() {
	if (world != nullptr) { // deregister call backlistener to avoid getting callbacks when recreating the world
		world->SetContactListener(nullptr);
	}


	toRemove.clear();
	sceneObjects.clear();
	physicsComponentLookup.clear();
	initPhysics();

	spriteAtlas = SpriteAtlas::create("asteroids.json", "asteroids.png");
	score = 0;
	auto spaceship = createGameObject();
	spaceship->objectType = PlayerShip;
	auto spaceshipSprite = spriteAtlas->get("playerShip1_blue.png");
	auto spriteObject = spaceship->addComponent<SpriteComponent>();
	spriteObject->setSprite(spaceshipSprite);

	spaceship->setPosition(windowSize * 0.5f);
	auto spaceshipPhys = spaceship->addComponent<PhysicsComponent>();
	spaceshipPhys->initCircle(b2_dynamicBody, spaceshipSprite.getSpriteSize().y / 2 / physicsScale, { spaceship->getPosition().x / physicsScale,spaceship->getPosition().y / physicsScale }, 1);
	auto controller = spaceship->addComponent<PlayerController>();

	auto spaceShipWeapon = spaceship->addComponent<WeaponComponent>();
	auto guiController = spaceship->addComponent<GuiController>();
	guiController->setFont(gameFont);

	for (size_t i = 0; i < 5; i++)
	{
		SpawnEnemy(ObjectType::AsteroidLarge);
		SpawnEnemy(ObjectType::AsteroidMedium);
		SpawnEnemy(ObjectType::AsteroidSmall);
	}

}

void AsteroidsGame::initPhysics() {
	delete world;
	world = new b2World(b2Vec2(0, 0));
	world->SetContactListener(this);

	if (doDebugDraw) {
		world->SetDebugDraw(&debugDraw);
	}
}

void AsteroidsGame::update(float deltaTime) {
	if (gameState != GameState::Running)
		return;

	enemySpawnTimer -= deltaTime;
	upgradeSpawnTimer -= deltaTime;

	if (gameState == GameState::Running) {
		updatePhysics();
		for (auto pair : toCreate) {
			SpawnEnemy(pair.first, pair.second);
		}
		for (auto o : toRemove) {
			auto found = std::find_if(sceneObjects.begin(), sceneObjects.end(), [&](std::shared_ptr<GameObject> obj) {return obj.get() == o; });
			if (found != sceneObjects.end()) {
				int index = found - sceneObjects.begin();
				sceneObjects[index] = sceneObjects.back();
			}
			sceneObjects.pop_back();
		}
		toRemove.clear();
		toCreate.clear();
	}
	for (int i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->update(deltaTime);
	}

	// Check whether to spawn an enemy, and reset the timer
	if (enemySpawnTimer <= 0.0f)
	{
		SpawnEnemy(ObjectType::AsteroidLarge);
		enemySpawnTimer = enemySpawnTimerReset;
	}

	// Check whether to spawn an upgrade, and reset the timer
	if (upgradeSpawnTimer <= 0.0f)
	{
		SpawnUpgrade();
		upgradeSpawnTimer = upgradeSpawnTimerReset;
	}
}

void AsteroidsGame::updatePhysics() {
	const float32 timeStep = 1.0f / 60.0f;
	const int positionIterations = 2;
	const int velocityIterations = 6;
	world->Step(timeStep, velocityIterations, positionIterations);

	for (auto phys : physicsComponentLookup) {
		if (phys.second->rbType == b2_staticBody) continue;
		auto position = phys.second->body->GetPosition();
		float angle = phys.second->body->GetAngle();
		auto gameObject = phys.second->getGameObject();

		//Screen wrapping based on the object's radius to ensure it's out of screen when teleporting
		auto rad = phys.second->circle->m_radius;
		if (position.x <= -rad) {
			position.x = wrapperSize.x + rad;
		}
		else if (position.x >= wrapperSize.x + rad) {
			position.x = -rad;
		}
		if (position.y <= -rad) {
			position.y = wrapperSize.y + rad;
		}
		else if (position.y >= wrapperSize.y + rad) {
			position.y = -rad;
		}
		phys.second->body->SetTransform(b2Vec2(position.x, position.y), angle);

		gameObject->setPosition(glm::vec2(position.x, position.y) * physicsScale);
		gameObject->setRotation(angle);
	}
}

void AsteroidsGame::render() {
	auto renderPass = RenderPass::create()
		.withCamera(camera)
		.withClearColor(true, { .20, .60, .86, 1 })
		.build();
	auto spriteBatchBuilder = SpriteBatch::create();

	for (int i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->renderSprite(spriteBatchBuilder);
	}

	auto spriteBatch = spriteBatchBuilder.build();
	renderPass.draw(spriteBatch);

	if (doDebugDraw) {
		world->DrawDebugData();
		renderPass.drawLines(debugDraw.getLines());
		debugDraw.clear();
	}

	for (auto& go : sceneObjects) {
		for (auto& comp : go->getComponents()) {
			comp->onGui();
		}
	}
}

void AsteroidsGame::keyEvent(SDL_Event& event) {
	for (auto& gameObject : sceneObjects) {
		for (auto& c : gameObject->getComponents()) {
			bool consumed = c->keyEvent(event);
			if (consumed) {
				return;
			}
		}
	}

	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
		case SDLK_d:
			// press 'd' for physics debug
			doDebugDraw = !doDebugDraw;
			if (doDebugDraw) {
				std::cout << "Enabling debug draw" << std::endl;
				world->SetDebugDraw(&debugDraw);
			}
			else {
				std::cout << "Disabling debug draw" << std::endl;
				world->SetDebugDraw(nullptr);
			}
			break;
		case SDLK_r:
			init();
			break;
		case SDLK_SPACE:
			if (gameState == GameState::GameOver) {
				init();
				gameState = GameState::Ready;
			}
			else if (gameState == GameState::Ready) {
				gameState = GameState::Running;
			}
			break;
		}
	}
}

void AsteroidsGame::incrementScore() {
	score += 1;
}

std::shared_ptr<GameObject> AsteroidsGame::SpawnEnemy(ObjectType objectType, glm::vec2 position) {
	auto enemy = createGameObject();
	auto spriteComp = enemy->addComponent<SpriteComponent>();
	auto physicsComponent = enemy->addComponent<PhysicsComponent>();
	auto controller = enemy->addComponent<EnemyController>();
	enemy->objectType = objectType;

	std::string spriteName;
	float radius;
	float density;
	glm::vec2 force;

	switch (objectType) {
	case AsteroidLarge:
		spriteName = "meteorBrown_big4.png";
		radius = 42;
		density = 7;
		force = glm::vec2{ Randomness::generateRandomFloat(50, 30),
									Randomness::generateRandomFloat(50, 30) };
		break;
	case AsteroidMedium:
		spriteName = "meteorBrown_med1.png";
		radius = 20;
		density = 7;
		force = glm::vec2{ Randomness::generateRandomFloat(30, 10),
						  Randomness::generateRandomFloat(30, 10) };
		break;
	case AsteroidSmall:
		spriteName = "meteorBrown_tiny1.png";
		radius = 10;
		density = 7;
		force = glm::vec2{ Randomness::generateRandomFloat(2),
						  Randomness::generateRandomFloat(2) };
		break;
	default:
		std::cout << "EnemyType: " << objectType << " Not covered in SpawnEnemy" << std::endl;
	}

	if (position.x == 0 && position.y == 0) { //The default position, i.e. no position was given to the method
		position = Randomness::generateEnemySpawnPoint(radius);
	}

	enemy->setPosition(position);
	auto sprite = spriteAtlas->get(spriteName);
	spriteComp->setSprite(sprite);

	physicsComponent->initCircle(b2_dynamicBody,
		radius / physicsScale,
		glm::vec2{
				   enemy->position.x / physicsScale,
				   enemy->position.y / physicsScale
		},
		density);
	auto direction = Randomness::generateSpawnDirection(enemy->position);
	physicsComponent->addForce(force * direction);

	return enemy;
}

#define PROJECTILE_SIZE 1.0f
#define PROJECTILE_SPEED 2.5f
#define PROJECTILE_LIFETIME 5.0f //Seconds
std::shared_ptr<GameObject> AsteroidsGame::SpawnProjectile(GameObject* shooter, float projectileSize, float projectileSpeed, float projectileLifetime) {
	auto projectile = createGameObject();
	projectile->setRotation(shooter->rotation);
	projectile->setPosition(shooter->position);
	projectile->objectType = PlayerBullet;
	projectile->addComponent<BulletController>();

	auto lifetimeComponent = projectile->addComponent<LifetimeComponent>();
	lifetimeComponent->setLifetime(PROJECTILE_LIFETIME * projectileLifetime);

	auto projectileSprite = spriteAtlas->get("laserBlue01.png");
	projectileSprite.setScale(projectileSprite.getScale() * PROJECTILE_SIZE * projectileSize);
	auto spriteComp = projectile->addComponent<SpriteComponent>();
	spriteComp->setSprite(projectileSprite);

	auto physics = projectile->addComponent<PhysicsComponent>();
	physics->initCircle(b2_dynamicBody, projectileSprite.getSpriteSize().x * projectileSprite.getScale().x / 2 / physicsScale, projectile->getPosition() / physicsScale, 1);
	glm::vec2 direction = glm::rotateZ(glm::vec3(0, PROJECTILE_SPEED * projectileSpeed, 0), glm::radians(projectile->rotation));
	physics->setLinearVelocity(direction);
	physics->setRotation(shooter->rotation);
	physics->setSensor(true);

	return projectile;
}

std::shared_ptr<GameObject> AsteroidsGame::SpawnUpgrade()
{
	auto upgrade = createGameObject();
	upgrade->setPosition(windowSize * 0.5f);
	upgrade->objectType = Upgrade;
	upgrade->addComponent<UpgradeController>();

	auto upgradeSprite = spriteAtlas->get("powerupYellow_star.png");
	auto spriteComp = upgrade->addComponent<SpriteComponent>();
	spriteComp->setSprite(upgradeSprite);

	auto physics = upgrade->addComponent<PhysicsComponent>();
	physics->initCircle(b2_staticBody, upgradeSprite.getSpriteSize().x / 2 / physicsScale, { windowSize.x / 2 / physicsScale, windowSize.y / 2 / physicsScale }, 1);
	physics->setSensor(true);

	return upgrade;
}

std::shared_ptr<GameObject> AsteroidsGame::createGameObject() {
	auto obj = std::shared_ptr<GameObject>(new GameObject());
	sceneObjects.push_back(obj);

	return obj;
}

void AsteroidsGame::registerPhysicsComponent(PhysicsComponent* r) {
	physicsComponentLookup[r->fixture] = r;
}

void AsteroidsGame::deregisterPhysicsComponent(PhysicsComponent* r) {
	auto iter = physicsComponentLookup.find(r->fixture);
	if (iter != physicsComponentLookup.end()) {
		physicsComponentLookup.erase(iter);
	}
	else {
		assert(false); // cannot find physics object
	}
}

void AsteroidsGame::scheduleForRemoval(GameObject* obj) {
	toRemove.push_back(obj);
}

void AsteroidsGame::scheduleForCreation(ObjectType type, glm::vec2 position) {
	toCreate.emplace_back(type, position);
}

void AsteroidsGame::BeginContact(b2Contact* contact) {
	b2ContactListener::BeginContact(contact);
	handleContact(contact, true);
}

void AsteroidsGame::EndContact(b2Contact* contact) {
	b2ContactListener::EndContact(contact);
	handleContact(contact, false);
}

void AsteroidsGame::handleContact(b2Contact* contact, bool begin) {
	auto fixA = contact->GetFixtureA();
	auto fixB = contact->GetFixtureB();
	auto physA = physicsComponentLookup.find(fixA);
	auto physB = physicsComponentLookup.find(fixB);

	if (physA != physicsComponentLookup.end() && physB != physicsComponentLookup.end()) {
		auto& aComponents = physA->second->getGameObject()->getComponents();
		auto& bComponents = physB->second->getGameObject()->getComponents();
		for (auto& c : aComponents) {
			if (begin) {
				c->onCollisionStart(physB->second);
			}
			else {
				c->onCollisionEnd(physB->second);
			}
		}
		for (auto& c : bComponents) {
			if (begin) {
				c->onCollisionStart(physA->second);
			}
			else {
				c->onCollisionEnd(physA->second);
			}
		}
	}
}

void AsteroidsGame::setGameState(GameState newState) {
	this->gameState = newState;
}

void AsteroidsGame::setupSounds() {
	// Audio setup
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

	soundMap.emplace(AsteroidExplosion, std::shared_ptr<Mix_Chunk>(Mix_LoadWAV("Sounds/explosion.wav")));
	soundMap.emplace(Shooting, std::shared_ptr<Mix_Chunk>(Mix_LoadWAV("Sounds/laserShoot.wav")));
	soundMap.emplace(SpaceshipExplosion, std::shared_ptr<Mix_Chunk>(Mix_LoadWAV("Sounds/spaceShipExplosion.wav")));
}

void AsteroidsGame::playSound(SoundEnum sound)
{
	std::shared_ptr<Mix_Chunk> chunk = soundMap.find(sound)->second;
	Mix_PlayChannel(-1, chunk.get(), 0);
}

int AsteroidsGame::getScore() {
	return score;
}

void AsteroidsGame::initGui() {
	auto fonts = ImGui::GetIO().Fonts;
	fonts->AddFontDefault();
	auto fontName = "Assets/Fastrace.ttf";
	int fontSize = 20;
	gameFont = fonts->AddFontFromFileTTF(fontName, fontSize);
}

int main() {
	new AsteroidsGame();
	return 0;
}
