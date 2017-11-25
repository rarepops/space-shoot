#include "SpaceShoot.hpp"
#include <ctime>

SpaceShoot::SpaceShoot()
{
    renderer.setWindowTitle("SpaceShoot");
    renderer.init(SDL_INIT_EVERYTHING, SDL_WINDOW_OPENGL);

    srand(time(NULL));

    init();
    camera.setWindowCoordinates();

    renderer.keyEvent = [&](SDL_Event& e) {
        onKey(e);
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

}

void SpaceShoot::update(float time)
{
}

void SpaceShoot::onKey(SDL_Event & event)
{

}

void SpaceShoot::render()
{

}
