#pragma once

#include "sre/Camera.hpp"
#include "glm/glm.hpp"
#include "Component.hpp"
#include <vector>

class FollowCamera : public Component
{
public:
    explicit FollowCamera(GameObject *gameObject);

    void update(float deltaTime) override;

    void init(GameObject* followObject, glm::vec2 offset = {0, 0});

    /* Sets the zoom level. */
    void setZoomLevel(int zoomLevel);

    /* Sets a custom zoom level. Lower = more zoomed out. */
    void setZoom(float zoom);
    float getZoom();

    /* Goes through the zoom levels. */
    void changeZoom();

    /* Shakes the camera by an ammount. */
    void shake(float amount = 33);

    bool FollowCamera::onKey(SDL_Event& keyEvent);
    sre::Camera& getCamera();
private:
    sre::Camera camera;
    GameObject* followObject;
    glm::vec2 offset;
    
    /* Lower numbers = the more it zooms out. */
    std::vector<float> zoomLevels = {0.5f, 1, 2, 0.25f};
    int currentZoomLevel = 0;
    float currentZoom = 0;
    float shakeAmount = 0;
};

