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

using namespace glm;
using namespace std;
using namespace sre;

const glm::vec2 SpaceShoot::windowSize(1000, 800);
SpaceShoot* SpaceShoot::instance = nullptr;
int SpaceShoot::PLAYER_GROUP = -1;
int SpaceShoot::ENEMY_GROUP = -2;
int SpaceShoot::enemiesKilled = 0;

int starsNumber = 500;
float gameBounds = 20000;

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
        star->setPosition(glm::vec2(rand() % (2 * (int)gameBounds) - ((int)gameBounds + 1), rand() % (2 * (int)gameBounds) - ((int)gameBounds + 1)));
        star->setRotation(rand() % 720 - 360);
    }

    // Spawn Player
    player = createGameObject();
    player->name = "Player";
    auto playerSprite = player->addComponent<SpriteComponent>();
    auto spaceShip = player->addComponent<ShipComponent>();
    spaceShip->setIsPlayer(true);
    spaceShip->getPhysicsComponent()->initBox(b2_dynamicBody, {0.5, 1}, player->getPosition() / physicsScale, 1, PLAYER_GROUP);
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



    // Spawn Enemies
    auto junk = createGameObject();
    junk->name = "Enemy";
    auto junkShip = junk->addComponent<ShipComponent>();
    auto junkSprite = junk->addComponent<SpriteComponent>();
    junk->setPosition(windowSize * 0.5f);


    junkSprite->setSprite(atlas->get("enemyspaceship.png"));
    junkShip->getPhysicsComponent()->initBox(b2_dynamicBody, {0.5, 1}, junk->getPosition() / physicsScale, 1,ENEMY_GROUP);
    auto turretControllerJunk = junk->addComponent<TurretController>();
    turretControllerJunk->init({
        {-39, 80},
        {-39, 38},
        {-44, -64},
        {39, 80}
    }, atlas->get("turret2.png"));

    /*turretControllerJunk->setSprite(atlas->get("turret2.png"));
    turretControllerJunk->setBulletSprite(atlas->get("particlered.png"), ENEMY_GROUP);
	turretControllerJunk->setAimAt(player);
    turretControllerJunk->initTurrets({
        {-39, 80},
        {-39, 38},
        {-44, -64},
        {39, 80},
        {39, 38},
        {44, -64}
    }
    );*/
    

    auto cam = createGameObject();
    cam->name = "Camera";
    this->camera = cam->addComponent<FollowCamera>();
    camera->init(player.get());
}

void SpaceShoot::initPhysics()
{
    float gravity = 0; // 9.8 m/s2
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
        gameObject->setRotation(angle);
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
            case SDLK_z:
                //camera->setZoomMode(!camera->isZoomMode());
                break;
            case SDLK_x:
                // press 'd' for physics debug
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

    //auto pos = camera->getGameObject()->getPosition();

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

    ImGui::NewFrame();
    ImVec2 playerWindowSize = {160, 100};
    ImGui::SetNextWindowPos(ImVec2((windowSize.x - playerWindowSize.x)*0.5f, (windowSize.y - playerWindowSize.y)), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(playerWindowSize, ImGuiSetCond_Always);
    ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

    ImGui::TextColored({1, 0.05f, 0.05f, 1}, "Hull: %3.2f%%", player->getComponent<ShipComponent>()->getHull()->capacityPercent() * 100);
    ImGui::TextColored({0, 0.5f, 1, 1}, "Shield: %3.2f%%", player->getComponent<ShipComponent>()->getShieldGenerator()->capacityPercent() * 100);
    ImGui::TextColored({1, 1, 0, 1}, "Energy: %3.2f%%", player->getComponent<ShipComponent>()->getEnergyGenerator()->capacityPercent() * 100);
    ImGui::TextColored({0.5f, 1, 0.5f, 0.8f}, "Score: %d", enemiesKilled);
    ImGui::TextColored({1, 1, 1, 0.2f}, "GO Instances: %d", GameObject::instances);

    ImGui::End();
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
