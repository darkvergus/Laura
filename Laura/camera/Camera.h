#pragma once
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "deltaLib/deltaLib.h"
#include <GLFW/glfw3.h>

enum MovementType {
    WORLD_RELATIVE = 1,
    CAMERA_RELATIVE = 0
};

class Camera
{
    //static struct cameraProperties {
    //    float Aspect;
    //    float FOV_rad;
    //};
    //
    ///**
    //* @brief The output of the camera
	// * This is literally all the shader needs from the camera
	// */
    //struct cameraOutput {
    //    float focalLength;
    //    glm::vec3 posVec;
    //    glm::mat3 u_ModelMatrix;
    //};

private:
    struct Flags {
        bool schedulePosUpdate = false;
        bool scheduleRotUpdate = false;
        bool scheduleScreenUpdate = false; // including change in Aspect, FOV, size of the screen
    };

    glm::vec3 defaultFwdVec = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 defaultUpVec = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 defaultRtVec = glm::cross(defaultFwdVec, defaultUpVec);

    glm::vec3 fwdVec = defaultFwdVec;
    glm::vec3 upVec = defaultUpVec;
    glm::vec3 rtVec = defaultRtVec;

    int sWidth;
    int sHeight;

    float Aspect;
    float FOV_rad;

    glm::mat3 u_ModelMatrix;

public:
    Camera(float FOV_deg, float ASPECT, DeltaTime& deltaTime);

    void Update();
    void ResetFlags();
   
    void updatePosition();
    void updateRotation();
    
    void setScreenDimentions(int newWidth, int newHeight);
    inline void setAspect(float newAspect) { Aspect = newAspect; }

    // getters
    inline glm::mat3 getModelMatrix() { return u_ModelMatrix; }

    inline int getScreenWidth() { return sWidth; }
    inline int getScreenHeight() { return sHeight; }

    inline float getAspect() const { return Aspect; }

    inline glm::vec3 getPos() { return posVec; }
    inline glm::vec3 getFwdVec() { return fwdVec; }
    inline glm::vec3 getUpVec() { return upVec; }
    inline glm::vec3 getRtVec() { return rtVec; }

    Flags flags;

    glm::vec3 newDeltaPos;

    float deltaRotAroundZ; // roll - around center
    float deltaRotAroundY; // yaw - left right
    float deltaRotAroundX; // pitch - up down 
    
    glm::vec3 posVec;
    float focalLength;

    float rotAroundZ = 0; // roll - around center
    float rotAroundY = 0; // yaw - left right
    float rotAroundX = 0; // pitch - up down

    bool FORWARD_KEY_ACTIVE    = false;
    bool BACKWARD_KEY_ACTIVE   = false;
    bool LEFT_KEY_ACTIVE       = false;
    bool RIGHT_KEY_ACTIVE      = false;
    bool UP_KEY_ACTIVE         = false;
    bool DOWN_KEY_ACTIVE       = false;
    bool ROLL_RIGHT_KEY_ACTIVE = false;
    bool ROLL_LEFT_KEY_ACTIVE  = false;

    struct CameraKeybinds {
        int FORWARD     = GLFW_KEY_W;
        int BACKWARD    = GLFW_KEY_S;
        int LEFT        = GLFW_KEY_A;
        int RIGHT       = GLFW_KEY_D;
        int UP          = GLFW_KEY_SPACE;
        int DOWN        = GLFW_KEY_LEFT_SHIFT;
        int ROLL_LEFT   = GLFW_KEY_Q;
        int ROLL_RIGHT  = GLFW_KEY_E;
        bool camera_keybind_window_active = false;
        int* keybind_to_be_changed;
    };

    CameraKeybinds cameraKeybinds;

    bool wasCameraInput = false;
    bool freeze = false;

    MovementType movementType = WORLD_RELATIVE;
    float sensitivity = 15.0f;
    float speed = 7.0f;
    DeltaTime& deltaTime;
    float FOV_deg;
};
