//
// Created by Morten Nobel-Jørgensen on 10/10/2017.
//

#include "FollowCamera.hpp"
#include "SpaceShoot.hpp"
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
        
        camera.setOrthographicProjection(SpaceShoot::windowSize.y / currentZoom, -1, 1);
        auto position = followObject->getPosition();

        position = followObject->getPosition() + offset;

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

void FollowCamera::setFollowObject(std::shared_ptr<GameObject> followObject, glm::vec2 offset)
{
    this->followObject = followObject;
    this->offset = offset;
}

void FollowCamera::setZoomLevel(int zoomLevel)
{
    if(zoomLevel >= 0 && zoomLevel <= zoomLevels.size())
    {
        currentZoomLevel = zoomLevel;
        setZoom(zoomLevels[currentZoomLevel]);
    }
}

void FollowCamera::setZoom(float zoom)
{
    currentZoom = zoom;
}

void FollowCamera::changeZoom()
{
    ++currentZoomLevel;
    currentZoomLevel %= zoomLevels.size();

    setZoom(zoomLevels[currentZoomLevel]);
}

float FollowCamera::getZoom()
{
	return currentZoom/2;
}
