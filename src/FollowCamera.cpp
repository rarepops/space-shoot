#include "FollowCamera.hpp"
#include "SpaceShoot.hpp"
#include <glm/gtc/random.hpp>
#include <iostream>

using namespace glm;

FollowCamera::FollowCamera(GameObject *gameObject)
    : Component(gameObject)
{
    camera.setWindowCoordinates();
    currentZoom = zoomLevels[currentZoomLevel];
}

sre::Camera &FollowCamera::getCamera()
{
    return camera;
}

void FollowCamera::update(float deltaTime)
{
    if(followObject != nullptr)
    {
        glm::vec2 shakeVector = {0, 0};
        if(shakeAmount > 0)
        {
            shakeVector = glm::circularRand(shakeAmount);

            // We diminish the shake amount each frame
            shakeAmount /= 3;
        }
        else
        {
            shakeAmount = 0;
        }

        camera.setOrthographicProjection(SpaceShoot::windowSize.y / currentZoom, -1, 1);
        auto position = followObject->getPosition();

        position = followObject->getPosition() + offset + shakeVector;

        vec3 eye(position, 0);
        vec3 at(position, -1);
        vec3 up(0, 1, 0);
        camera.lookAt(eye, at, up);
    }
}

bool FollowCamera::onKey(SDL_Event& keyEvent)
{
    switch(keyEvent.key.keysym.sym)
    {
        case SDLK_z:
        {
            if(keyEvent.type == SDL_KEYUP)
            {
                changeZoom();
            }
        }
        break;
    }

    return false;
}

void FollowCamera::init(GameObject* followObject, glm::vec2 offset)
{
    this->followObject = followObject;
    this->offset = offset;
}

void FollowCamera::setZoomLevel(int zoomLevel)
{
    // We clamp the zoom level so it stays within bounds
    zoomLevel %= zoomLevels.size();

    currentZoomLevel = zoomLevel;
    setZoom(zoomLevels[currentZoomLevel]);
}

void FollowCamera::setZoom(float zoom)
{
    currentZoom = zoom;
}

void FollowCamera::changeZoom()
{
    ++currentZoomLevel;

    // We clamp the zoom level so it stays within bounds
    currentZoomLevel %= zoomLevels.size();
    setZoom(zoomLevels[currentZoomLevel]);
}

float FollowCamera::getZoom()
{
    return currentZoom;
}

void FollowCamera::shake(float amount)
{
    shakeAmount += amount;
}

