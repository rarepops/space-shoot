#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"
#include <vector>
#include "Box2D/Dynamics/b2World.h"
#include "Box2DDebugDraw.hpp"
#include "GameObject.hpp"
#include "FollowCamera.hpp"
#include "SDL_mixer.h"

class PhysicsComponent;

class SpaceShoot : public b2ContactListener
{
public:
    SpaceShoot();
    static constexpr float32 timeStep = 1.0f / 60.0f;
    const float physicsScale = 100;
    static SpaceShoot* instance;
    static const glm::vec2 windowSize;

    std::shared_ptr<GameObject> createGameObject();
    void BeginContact(b2Contact *contact) override;
    void EndContact(b2Contact *contact) override;

    /* Will spawn an enemy if enemy limit is not reached. */
    void SpawnEnemies();

    /* Will spawn/respawn the player. */
    void SpawnPlayer();
    void resetGame();

    std::shared_ptr<GameObject> getPlayer();

    std::shared_ptr<sre::SpriteAtlas> atlas;
    std::shared_ptr<FollowCamera> camera;

    /* Collision layers. */
    static int PLAYER_GROUP;
    static int ENEMY_GROUP;

    bool gameEnded = false;
private:
    glm::vec4 bgColor;
    sre::SDLRenderer renderer;
    Mix_Music *music;

    void init();
    void initPhysics();
    void update(float time);
    void updatePhysics();
    void onKey(SDL_Event &event);
    void onMouse(SDL_Event& event);
    void render();

    std::vector<std::shared_ptr<GameObject>> sceneObjects;

    void registerPhysicsComponent(PhysicsComponent *r);
    void deregisterPhysicsComponent(PhysicsComponent *r);
    void handleContact(b2Contact *contact, bool begin);
    std::map<b2Fixture*, PhysicsComponent*> physicsComponentLookup;

    std::shared_ptr<GameObject> player;
    b2World *world = nullptr;
    Box2DDebugDraw debugDraw;
    bool isDebugDraw = false;
    int currentEnemies = 0;
    int enemiesKilled = 0;
    float elapsedTime = 0;

    friend class PhysicsComponent;
    friend class ShipComponent;
    friend class BulletComponent;
};