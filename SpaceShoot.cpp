#include <sre/Profiler.hpp>
#include <ctime>
#include "sre/RenderPass.hpp"
#include "PhysicsComponent.hpp"
#include "SpriteComponent.hpp"
#include "Box2D/Dynamics/Contacts/b2Contact.h"
#include "SpaceShoot.hpp"
#include "ShipComponent.hpp"
#include "TurretController.hpp"
#include <iostream>
#include "glm/gtc/random.hpp"

// Controls: W/S - move forwards/backwards
// A/D - rotate left/right
// mouse left click - shoot
// x - debug draw
// z - camera zoom

using namespace glm;
using namespace std;
using namespace sre;

const glm::vec2 SpaceShoot::windowSize(1000, 800);
SpaceShoot* SpaceShoot::instance = nullptr;
int SpaceShoot::PLAYER_GROUP = -1;
int SpaceShoot::ENEMY_GROUP = -2;

int starsNumber = 600;
float gameBounds = 10000;

SpaceShoot::SpaceShoot() : debugDraw(physicsScale)
{
    instance = this;
    srand(time(NULL));

    renderer.setWindowSize(windowSize);
    renderer.setWindowTitle("SpaceShooter");
    renderer.init(SDL_INIT_EVERYTHING, SDL_WINDOW_OPENGL);

    bool useVsync = true;

    bgColor = glm::vec4(0.01, 0.01, 0.01, 1);

    atlas = SpriteAtlas::create("spaceshooter.json", "spaceshooter.png");

    init();

    renderer.keyEvent = [&](SDL_Event& e) {
        onKey(e);
    };
    renderer.mouseEvent = [&](SDL_Event& e) {
        onMouse(e);
    };
    renderer.frameUpdate = [&](float deltaTime) {
        update(deltaTime);
    };
    renderer.frameRender = [&]() {
        render();
    };

    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        cout << "Cannot initialize audio output" << endl;
        return;
    }

    music = Mix_LoadMUS("danger.ogg");

    if(music == nullptr)
    {
        cout << "Cannot load music!" << endl;
    }

    Mix_FadeInMusic(music, -1, 2000);
    // Set the volume to half.
    Mix_VolumeMusic(64);

    renderer.startEventLoop();
}

void SpaceShoot::init()
{
    initPhysics();

    // Spawn Stars
    Sprite starSprites[3] = {atlas->get("star1.png"), atlas->get("star2.png"), atlas->get("star3.png")};
    for(int i = 0; i < starsNumber; ++i)
    {
        std::shared_ptr<GameObject> star = createGameObject();
        auto starSprite = star->addComponent<SpriteComponent>();
        starSprite->setSprite(starSprites[rand() % 3]);
        star->setPosition(glm::vec2(rand() % (2 * (int)gameBounds) - ((int)gameBounds + 1),
            rand() % (2 * (int)gameBounds) - ((int)gameBounds + 1)));
        star->setRotation(rand() % 720 - 360);
    }

    SpawnPlayer();

    auto cam = createGameObject();
    cam->name = "Camera";
    this->camera = cam->addComponent<FollowCamera>();
    camera->init(player.get());
}

void SpaceShoot::initPhysics()
{
    float gravity = 0;
    delete world;
    world = new b2World(b2Vec2(0, gravity));
    world->SetContactListener(this);

    if(isDebugDraw)
    {
        world->SetDebugDraw(&debugDraw);
    }
}

void SpaceShoot::update(float time)
{
    world->SetContactListener(nullptr);
    sceneObjects.erase(std::remove_if(sceneObjects.begin(), sceneObjects.end(), [](std::shared_ptr<GameObject> object) {
        return object.get()->destroyed;
    }), sceneObjects.end());

    world->SetContactListener(this);

    updatePhysics();
    if(time > 0.03) // if framerate approx 30 fps then run two physics steps
    {
        updatePhysics();
    }

    for(int i = 0; i < sceneObjects.size(); i++)
    {
        sceneObjects[i]->update(time);
    }

    if(!gameEnded)
    {
        elapsedTime += time;
    }

    SpawnEnemies();
}

void SpaceShoot::updatePhysics()
{
    const int positionIterations = 4;
    const int velocityIterations = 12;
    world->Step(timeStep, velocityIterations, positionIterations);

    for(auto phys : physicsComponentLookup)
    {
        PhysicsComponent* physicsComponent = phys.second;
        if(physicsComponent->isAutoUpdate() == false) continue;
        auto position = physicsComponent->getBody()->GetPosition();
        float angle = physicsComponent->getBody()->GetAngle();
        auto gameObject = physicsComponent->getGameObject();
        gameObject->setPosition(glm::vec2(position.x * physicsScale, position.y * physicsScale));
        gameObject->setRotation(degrees(angle));
    }
}

void SpaceShoot::onKey(SDL_Event& event)
{
    for(auto& gameObject : sceneObjects)
    {
        for(auto& c : gameObject->getComponents())
        {
            bool consumed = c->onKey(event);
            if(consumed)
            {
                return;
            }
        }
    }

    if(event.type == SDL_KEYDOWN)
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_ESCAPE:
            {
                if(gameEnded)
                {
                    resetGame();
                    break;
                }
            }
            case SDLK_x:
            {
                isDebugDraw = !isDebugDraw;
                if(isDebugDraw)
                {
                    world->SetDebugDraw(&debugDraw);
                }
                else
                {
                    world->SetDebugDraw(nullptr);
                }
                break;
            }
        }
    }
}

void SpaceShoot::onMouse(SDL_Event& event)
{
    for(int i = 0; i < sceneObjects.size(); ++i)
    {
        for(auto& c : sceneObjects[i]->getComponents())
        {
            bool consumed = c->onMouse(event);
            if(consumed)
            {
                return;
            }
        }
    }
}

void SpaceShoot::render()
{
    auto rp = RenderPass::create()
        .withCamera(camera->getCamera())
        .withClearColor(true, bgColor)
        .build();


    if(isDebugDraw)
    {
        static Profiler profiler;
        profiler.update();
        profiler.gui(false);
    }

    auto spriteBatchBuilder = SpriteBatch::create();
    for(auto& go : sceneObjects)
    {
        go->renderSprite(spriteBatchBuilder);
    }

    auto sb = spriteBatchBuilder.build();
    rp.draw(sb);

    if(isDebugDraw)
    {
        world->DrawDebugData();
        rp.drawLines(debugDraw.getLines());
        debugDraw.clear();
    }
    else
    {
        ImGui::NewFrame();
        ImVec2 playerWindowSize = {160, 100};
        ImGui::SetNextWindowPos(ImVec2((windowSize.x - playerWindowSize.x) * 0.5f, (windowSize.y - playerWindowSize.y)),
            ImGuiSetCond_Always);
        ImGui::SetNextWindowSize(playerWindowSize, ImGuiSetCond_Always);
        ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

        if(!gameEnded)
        {
            ImGui::TextColored({1, 0.05f, 0.05f, 1}, "Hull: %3.2f%%", player->getComponent<ShipComponent>()->getHull()->capacityPercent() * 100);
            ImGui::TextColored({0, 0.5f, 1, 1}, "Shield: %3.2f%%", player->getComponent<ShipComponent>()->getShieldGenerator()->capacityPercent() * 100);
            ImGui::TextColored({1, 1, 0, 1}, "Energy: %3.2f%%", player->getComponent<ShipComponent>()->getEnergyGenerator()->capacityPercent() * 100);
        }
        else
        {
            ImGui::TextColored({1, 1, 1, 1}, "GAME OVER");
            ImGui::TextColored({1, 1, 1, 1}, "PRESS ESC TO RESTART");
            ImGui::TextColored({1, 1, 1, 1}, " ");
        }

        ImGui::TextColored({0.5f, 1, 0.5f, 0.8f}, "Score: %d | Time: %.1f", enemiesKilled, elapsedTime);
        ImGui::TextColored({1, 1, 1, 0.2f}, "GO Instances: %d", GameObject::instances);

        ImGui::End();
    }
}

void SpaceShoot::registerPhysicsComponent(PhysicsComponent* r)
{
    physicsComponentLookup[r->getFixture()] = r;
}

void SpaceShoot::deregisterPhysicsComponent(PhysicsComponent* r)
{
    auto iter = physicsComponentLookup.find(r->getFixture());
    physicsComponentLookup.erase(iter);
}

void SpaceShoot::BeginContact(b2Contact* contact)
{
    b2ContactListener::BeginContact(contact);
    handleContact(contact, true);
}

void SpaceShoot::EndContact(b2Contact* contact)
{
    b2ContactListener::EndContact(contact);
    handleContact(contact, false);
}

void SpaceShoot::SpawnEnemies()
{
    if(maxEnemies >= currentEnemies)
    {
        ++currentEnemies;

        auto junk = createGameObject();
        junk->name = "Enemy";
        auto junkShip = junk->addComponent<ShipComponent>();
        auto junkSprite = junk->addComponent<SpriteComponent>();
        glm::vec2 pos = glm::vec2((rand() % (int)gameBounds) - gameBounds / 2, (rand() % (int)gameBounds) - gameBounds / 2);
        junk->setPosition(pos);

        junkShip->init(0, 1800, 1400, 20, 2000, 90);
        //junkShip->init(10, 1, 1, 1, 5000, 50);

        junkSprite->setSprite(atlas->get("enemystation.png"));
        junkShip->getPhysicsComponent()->initBox(b2_kinematicBody, {0.3, 0.51}, junk->getPosition() / physicsScale, 1, ENEMY_GROUP);
        auto turretControllerJunk = junk->addComponent<TurretController>();
        turretControllerJunk->setTarget(player);
        turretControllerJunk->init({
            {0, 0},
            {0, -28},
            {0, 28}
        }, atlas->get("turret2.png"));
    }
}


void SpaceShoot::SpawnPlayer()
{
    if(!player)
    {
        player = createGameObject();
        player->name = "Player";
        auto playerSprite = player->addComponent<SpriteComponent>();
        auto spaceShip = player->addComponent<ShipComponent>();
        spaceShip->setIsPlayer(true);

        spaceShip->init(100, 4000, 2500, 45, 5000, 125);
        //spaceShip->init(10, 1, 1, 1, 5000, 50);

        spaceShip->getPhysicsComponent()->initBox(b2_dynamicBody, {0.63, 1.15}, player->getPosition() / physicsScale, 1,
            PLAYER_GROUP);
        auto sprite = atlas->get("playerspaceship.png");
        playerSprite->setSprite(sprite);
        auto turretController = player->addComponent<TurretController>();
        turretController->init({
            {-39, 80},
            {-39, 38},
            {-44, -64},
            {39, 80},
            {39, 38},
            {44, -64}
        }, atlas->get("turret1.png"));
    }
    else
    {
        player->getComponent<TurretController>()->toggleHideTurrets(0);
        player->getComponent<SpriteComponent>()->setSprite(atlas->get("playerspaceship.png"));
        player->getComponent<ShipComponent>()->init(100, 4000, 2500, 45, 5000, 125);
        
        // Does not work V because PhysicsComponent
        player->setPosition(glm::vec2());
        player->getComponent<PhysicsComponent>()->moveTo(glm::vec2(0, 0));
    }
}

void SpaceShoot::resetGame()
{
    gameEnded = !gameEnded;
    enemiesKilled = 0;
    elapsedTime = 0;
    SpawnPlayer();
}

shared_ptr<GameObject> SpaceShoot::getPlayer()
{
    return player;
}


void SpaceShoot::handleContact(b2Contact* contact, bool begin)
{
    auto fixA = contact->GetFixtureA();
    auto fixB = contact->GetFixtureB();
    auto physA = physicsComponentLookup.find(fixA);
    auto physB = physicsComponentLookup.find(fixB);
    if(physA != physicsComponentLookup.end() && physB != physicsComponentLookup.end())
    {
        auto& aComponents = physA->second->getGameObject()->getComponents();
        auto& bComponents = physB->second->getGameObject()->getComponents();
        for(auto& c : aComponents)
        {
            if(begin)
            {
                c->onCollisionStart(physB->second);
            }
            else
            {
                c->onCollisionEnd(physB->second);
            }
        }
        for(auto& c : bComponents)
        {
            if(begin)
            {
                c->onCollisionStart(physA->second);
            }
            else
            {
                c->onCollisionEnd(physA->second);
            }
        }
    }
}

std::shared_ptr<GameObject> SpaceShoot::createGameObject()
{
    auto obj = shared_ptr<GameObject>(new GameObject());
    sceneObjects.push_back(obj);
    return obj;
}
