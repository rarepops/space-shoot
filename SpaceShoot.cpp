#include <sre/Profiler.hpp>
#include <ctime>

#include "sre/RenderPass.hpp"
#include "PhysicsComponent.hpp"
#include "SpriteComponent.hpp"
#include "Box2D/Dynamics/Contacts/b2Contact.h"
#include "SpaceShoot.hpp"
#include "SpaceShipComponent.hpp"
#include "ShipComponent.hpp"
#include "TurretController.hpp"
#include <iostream>

using namespace glm;
using namespace std;
using namespace sre;

const glm::vec2 SpaceShoot::windowSize(800, 600);
SpaceShoot* SpaceShoot::instance = nullptr;
int SpaceShoot::PLAYER_GROUP = -1;

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

	renderer.keyEvent = [&](SDL_Event& e)
	{
		onKey(e);
	};
	renderer.mouseEvent = [&](SDL_Event& e)
	{
		onMouse(e);
	};
	renderer.frameUpdate = [&](float deltaTime)
	{
		update(deltaTime);
	};
	renderer.frameRender = [&]()
	{
		render();
	};

	renderer.startEventLoop();
}

shared_ptr<GameObject> player;

void SpaceShoot::init()
{
	initPhysics();

	player = createGameObject();
	player->name = "Player";
	auto playerSprite = player->addComponent<SpriteComponent>();
	auto spaceShip = player->addComponent<ShipComponent>();
	auto sprite = atlas->get("playerspaceship.png");
	playerSprite->setSprite(sprite);
	auto turretController = player->addComponent<TurretController>();
	turretController->setSprite(atlas->get("turret1.png"));
	turretController->offsetTurrets(
		{-39,80},
		{-39,38},
		{-44,-64},
		{ 39,80 },
		{39,38},
		{44,-64}
	);
	turretController->initTurrets();
	turretController->setBulletSprite(atlas->get("bulletstandard.png"));

	auto junk = createGameObject();
	auto junkSprite = junk->addComponent<SpriteComponent>();
	junk->setPosition(windowSize * 0.5f);
	junkSprite->setSprite(sprite);
	auto comp = junk->addComponent<PhysicsComponent>();
	comp->initBox(b2_staticBody, {0.20, 0.20}, junk->getPosition() / physicsScale, 1);


	auto cam = createGameObject();
	cam->name = "Camera";
	this->camera = cam->addComponent<FollowCamera>();
	camera->init(player);
}

void SpaceShoot::initPhysics()
{
	float gravity = 0; // 9.8 m/s2
	delete world;
	world = new b2World(b2Vec2(0, gravity));
	world->SetContactListener(this);

	if (isDebugDraw)
	{
		world->SetDebugDraw(&debugDraw);
	}
}

void SpaceShoot::update(float time)
{
	sceneObjects.erase(std::remove_if(sceneObjects.begin(), sceneObjects.end(), [](std::shared_ptr<GameObject> object)
                                  {
	                                  return object.get()->destroyed;
                                  }), sceneObjects.end());

	updatePhysics();
	if (time > 0.03) // if framerate approx 30 fps then run two physics steps
	{
		updatePhysics();
	}

	for (int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i]->update(time);
	}
}

void SpaceShoot::updatePhysics()
{
	const int positionIterations = 4;
	const int velocityIterations = 12;
	world->Step(timeStep, velocityIterations, positionIterations);

	for (auto phys : physicsComponentLookup)
	{
		PhysicsComponent* physicsComponent = phys.second;
		if (physicsComponent->isAutoUpdate() == false) continue;
		auto position = physicsComponent->getBody()->GetPosition();
		float angle = physicsComponent->getBody()->GetAngle();
		auto gameObject = physicsComponent->getGameObject();
		gameObject->setPosition(glm::vec2(position.x * physicsScale, position.y * physicsScale));
		gameObject->setRotation(angle);
	}
}

void SpaceShoot::onKey(SDL_Event& event)
{
	for (auto& gameObject : sceneObjects)
	{
		for (auto& c : gameObject->getComponents())
		{
			bool consumed = c->onKey(event);
			if (consumed)
			{
				return;
			}
		}
	}

	if (event.type == SDL_KEYDOWN)
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_z:
			//camera->setZoomMode(!camera->isZoomMode());
			break;
		case SDLK_x:
			// press 'd' for physics debug
			isDebugDraw = !isDebugDraw;
			if (isDebugDraw)
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
	for (int i = 0; i < sceneObjects.size(); ++i)
	{
		for (auto& c : sceneObjects[i]->getComponents())
		{
			bool consumed = c->onMouse(event);
			if (consumed)
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

	if (isDebugDraw)
	{
		static Profiler profiler;
		profiler.update();
		profiler.gui(false);
	}

	auto spriteBatchBuilder = SpriteBatch::create();
	for (auto& go : sceneObjects)
	{
		go->renderSprite(spriteBatchBuilder);
	}

	auto sb = spriteBatchBuilder.build();
	rp.draw(sb);

	if (isDebugDraw)
	{
		world->DrawDebugData();
		rp.drawLines(debugDraw.getLines());
		debugDraw.clear();
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
	//	b2ContactListener::EndContact(contact);
	//	handleContact(contact, false);
}


void SpaceShoot::handleContact(b2Contact* contact, bool begin)
{
    auto fixA = contact->GetFixtureA();
    auto fixB = contact->GetFixtureB();
    auto physA = physicsComponentLookup.find(fixA);
    auto physB = physicsComponentLookup.find(fixB);
    if(physA != physicsComponentLookup.end() && physB != physicsComponentLookup.end())
    {
        auto & aComponents = physA->second->getGameObject()->getComponents();
        auto & bComponents = physB->second->getGameObject()->getComponents();
        for(auto & c : aComponents)
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
        for(auto & c : bComponents)
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
