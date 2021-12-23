#pragma once

#include <vector>
#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"
#include "Box2D/Dynamics/b2World.h"
#include "GameObject.hpp"
#include "ObjectType.hpp"
#include "GuiController.hpp"
//#include "BackgroundComponent.hpp"
#include "Box2DDebugDraw.hpp"
#include "Component.hpp"
#include "PhysicsComponent.hpp"
#include "SoundEnum.hpp"
#include <SDL_mixer.h>

/*
* AsteroidsGame is the core class running the game and is in charge of
*  creating game objects, creating and storing the sprite atlas,
*  playing sounds, marking objects for creation and deleting,
*  rendering objects, propagating user input and more.
* Game state is an enum to keep track of the current state of the game,
*  whether that is Ready (after restarting the game), Running or 
*  GameOver (player has died and graphics and physics are no longer calculated).
* 
* AsteroidsGame inherits from b2ContactListener as the game's physics are
*  implemented with Box2D.
* 
* The source file also has a main() function definition to initialise and start
*  the game.
*/

// Forward declaration
class GameObject;
class PhysicsComponent;


enum class GameState {
	Ready,
	Running,
	GameOver
};

class AsteroidsGame : public b2ContactListener {
public:
	AsteroidsGame();
	static const glm::vec2 windowSize;
	static const glm::vec2 wrapperSize;
	std::shared_ptr<sre::SpriteAtlas> spriteAtlas;
	static std::shared_ptr<AsteroidsGame> instance;
	std::shared_ptr<GameObject> createGameObject();									 // Creates an empty game object
	std::shared_ptr<GameObject> SpawnEnemy(ObjectType,								 // Spawns an enemy dependent on ObjectType
											glm::vec2 position = glm::vec2{ 0,0 });	 // at position
																					 // If the ObjectType provided is an asteroid
																					 //  above the smallest, a one size smaller
																					 //  asteroid is created.

	std::shared_ptr<GameObject> SpawnProjectile(GameObject* shooter,				 // Creates a projectile based on the shooter
												float projectileSize,				 // with using projectileSize,
												float projectileSpeed,				 // projectilSpeed and
												float projectileLifetime);			 // projectileLifetime as factors for
																					 //  these properties.

	std::shared_ptr<GameObject> SpawnUpgrade();										 // Creates a random upgrade

	void playSound(SoundEnum sound);												 // Plays a sound

	int getScore();
	void incrementScore();

	void BeginContact(b2Contact* contact) override;									 // Implementing collision starting from Box2D
	void EndContact(b2Contact* contact) override;									 // Implementing collision ending from Box2D

	void setGameState(GameState newState);

	void scheduleForRemoval(GameObject* obj);										 // Schedules an object to be removed on the next frame
	void scheduleForCreation(ObjectType type, glm::vec2 position);					 // Schedules an object to be created next frame

private:
	void init();																	 // Initialises the game's starting state
																					 // Used to e.g. reset the game on restart

	void initGui();																	 // Initialises GUI font family and size
	void initPhysics();																 // Initialises the Box2D physics world
	void setupSounds();																 // Opens the SDL_Mixer audio and 
																					 // sets up a map of sounds to be played

	void update(float deltaTime);													 // The update call back function 
																					 //  updating the physics via updatePhysics,
																					 //  creating such scheduled objects, 
																					 //  removing such scheduled objects, 
																					 //  and spawning enemies and upgrades on timers

	void render();																	 // The render call back function handling
																					 //  making a RenderPass, a SpriteBatch builder
																					 //	 and rendering all the objects and GUI

	void keyEvent(SDL_Event& event);												 // Propagates user input to the components of
																					 //  game objects for handling, and handles
																					 //  static game wide input (e.g. restart or debug draw)

	void handleContact(b2Contact* contact, bool begin);								 // Handles colllision detection between physics objects
																					 //  propagating collisions to components via onCollisionStart
																					 //  and onCollisionEnd as appropriate. Called by BeginContact() and EndContact().
	sre::Camera camera;

	std::vector<std::shared_ptr<GameObject>> sceneObjects;							 // A list of shared poitners to all the objects in the game
	sre::SDLRenderer r;
	int score = 0;

	void updatePhysics();															 // Updates the physics of all physics simulated game objects
	b2World* world = nullptr;
	static const float physicsScale;												 // The scale factor between pixels and physics space
	void registerPhysicsComponent(PhysicsComponent* r);
	void deregisterPhysicsComponent(PhysicsComponent* r);
	std::map<b2Fixture*, PhysicsComponent*> physicsComponentLookup;
	Box2DDebugDraw debugDraw;
	bool doDebugDraw = false;
	GameState gameState = GameState::Running;
	friend class PhysicsComponent;

	std::vector<GameObject*> toRemove;
	std::vector<std::pair<ObjectType, glm::vec2>> toCreate;

	ImFont* gameFont;

	float enemySpawnTimerReset = 2.0f;												 // The time in seconds between spawning enemies
	float enemySpawnTimer = enemySpawnTimerReset;									 // The time in seconds until next enemy should spawn

	float upgradeSpawnTimerReset = 15.0f;											 // The time in seconds between spawning upgrades
	float upgradeSpawnTimer = upgradeSpawnTimerReset;								 // The time in seconds until next upgrade should spawn

	// Sound references																 // Map from sound types to SDL_Mixer chunks, the actual sounds to play
	std::map<SoundEnum, std::shared_ptr<Mix_Chunk>> soundMap = std::map<SoundEnum, std::shared_ptr<Mix_Chunk>>();
};
